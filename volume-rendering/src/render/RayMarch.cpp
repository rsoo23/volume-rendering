#include "pch.h"
#include "scene_objects/Light.hpp"
#include "scene_objects/Sphere.hpp"
#include "math/RandomNumberGenerator.hpp"
#include "math/Noise.hpp"
#include "RayMarch.hpp"

namespace {
	float smoothStep(float low, float high, float x) {
		float t = std::clamp((x - low) / (high - low), 0.f, 1.f);
		return t * t * (3.0 - (2.0 * t));
	}

	// FBM (Fractal Brownian Motion):
	// fractal pattern that the sum of procedural noise layers, whose frequency and amplitude vary from layer to layer
	// - octaves: number of layers
	// - lacunarity: gap between successive frequencies
	// - h: fractal increment parameter
	float sampleFBMDensity(const FBMConfig& config, Vector3 point, Vector3 center, float radius) {
		float fbmResult = 0;

		Vector3 vp = Vector3Subtract(point, center);
		float dist = std::min(1.f, Vector3Length(vp) / radius);
		float falloff = smoothStep(config.smoothStepLow, config.smoothStepHigh, dist);
		vp = Vector3Scale(vp, config.freq);

		for (size_t k = 0; k < config.octaves; k++) {
			fbmResult += perlinNoise(vp.x, vp.y, vp.z) * pow(config.lacunarity, -config.h * k);
			vp = Vector3Scale(vp, config.lacunarity);
		}

		// clip negative values
		return std::max(0.f, fbmResult) * (1 - falloff);
	}

	float samplePerlinNoiseDensity(Vector3 point) {
		float freq = 1.f;
		return (1 + perlinNoise(point.x * freq, point.y * freq, point.z * freq)) * 0.5f;
	}

	// 1 / (4 * PI): represents the phase function where all out-scattered directions are equally likely to be chosen
	// g (-1 <= g <= 1): the asymmetry factor controls whether light is out-scattered in the forward / backward direction
	// - -1 < g < 0: scattered backward
	// - g = 0: equals to 1 / (4 * PI)
	// - 0 < g < 1: scattered forward
	// cosTheta: cosine of angle between light direction (object to light) and view direction (object to camera)
	float henyeyGreensteinPhase(float g, float cosTheta) {
		float denom = 1 + g * g - 2 * g * cosTheta;
		return 1 / (4 * PI) * (1 - g * g) / (denom * sqrtf(denom));
	}
}

// in-scattering: 
// - when light that is not traveling to the eye scatters against a particle, sending a portion of light to the eye
// - gather all light from all directions that isn't meant for you and return some in your direction

// out-scattering:
// - when light that is traveling to the eye scatters against a particle, sending a portion of light to the eye
// - light is traveling towards you but some light is scattered in random directions, some will be in your direction

CustomColor RayMarch::backward(
	const RayMarchConfig& config,
	const Ray& ray, float t0, float t1, 
	const Light& light,
	const IVolumeObject& volume,
	CustomColor bgColor
) {
	const int steps = (int)((t1 - t0) / config.stepSize);
	CustomColor finalTransmittedLightSourceColor;

	const VolumeMaterial material = volume.getMaterial();
	const float extinctionCoeff = (material.absorptionCoeff + material.scatteringCoeff) * material.density;

	// transmittance per step in the volume
	const float stepVolTransmittance = exp(-config.stepSize * extinctionCoeff);

	// precalculate the final bg color that is transmitted through the volume
	const CustomColor finalBGColor = bgColor * pow(stepVolTransmittance, steps);

	const Vector3 lightRayDir = light.getDirection();

	for (int step = 0; step < steps; step++) {
		// t value of the light ray origin (steps start from t1 and go backward)
		const float lightRayOriginT = t1 - (step * config.stepSize) - (0.5f * config.stepSize);
		const Vector3 lightRayOrigin = Vector3Add(ray.position, Vector3Scale(ray.direction, lightRayOriginT));
		const Ray lightRay = { lightRayOrigin, lightRayDir };
		float lightT0, lightT1;

		// in-scattering
		if (volume.intersect(lightRay, lightT0, lightT1)) {
			const float cosTheta = Vector3DotProduct(lightRay.direction, Vector3Scale(ray.direction, -1));
			const float hg = henyeyGreensteinPhase(config.asymmetryFactor, cosTheta);
			// transmittance through the volume from the lightT1 (point where the light ray intersects the sphere) and the light ray origin
			const float volTransmittance = exp(-lightT1 * extinctionCoeff);
			// color of the transmitted / attenuated light that hits the light ray origin
			const CustomColor transmittedLightSourceColor = light.getColor() * config.stepSize * volTransmittance * material.scatteringCoeff * material.density * hg;

			finalTransmittedLightSourceColor += transmittedLightSourceColor;
		}

		finalTransmittedLightSourceColor *= stepVolTransmittance;
	}
	return finalBGColor + finalTransmittedLightSourceColor;
}

CustomColor RayMarch::forward(
	const RayMarchConfig& rmConfig,
	const FBMConfig& fbmConfig,
	const Ray& ray, float t0, float t1, 
	const Light& light,
	const IVolumeObject& volume,
	RandomNumberGenerator& rng,
	CustomColor bgColor
) {
	const int steps = (int)((t1 - t0) / rmConfig.stepSize);
	CustomColor finalTransmittedLightSourceColor;

	VolumeMaterial material = volume.getMaterial();
	const float extinctionCoeff = (material.absorptionCoeff + material.scatteringCoeff);

	float accumulatedStepVolTransmittance = 1.f;

	const Vector3 lightRayDir = light.getDirection();

	const Vector3 sphereCenter = volume.getCenter();
	const float sphereRadius = volume.getRadius();

	for (int step = 0; step < steps; step++) {
		// t value of the light ray origin (steps start from t1 and go forward)
		const float lightRayOriginT = t0 + (step + rng.uniform<float>(0.f, 1.f)) * rmConfig.stepSize;
		const Vector3 lightRayOrigin = Vector3Add(ray.position, Vector3Scale(ray.direction, lightRayOriginT));
		const Ray lightRay = { lightRayOrigin, lightRayDir };
		float lightT0, lightT1;

		// sample density of the light ray origin using Perlin noise
		float density = sampleFBMDensity(fbmConfig, lightRayOrigin, sphereCenter, sphereRadius);

		// transmittance per step in the volume
		const float stepVolTransmittance = exp(-density * rmConfig.stepSize * extinctionCoeff);

		accumulatedStepVolTransmittance *= stepVolTransmittance;

		// in-scattering
		if (volume.intersect(lightRay, lightT0, lightT1)) {
			const float cosTheta = Vector3DotProduct(lightRayDir, Vector3Scale(ray.direction, -1));
			const float hg = henyeyGreensteinPhase(rmConfig.asymmetryFactor, cosTheta);

			// calculate the opticalDepth for the light ray (ray march along light ray)
			const int opticalDepthSteps = (int)(lightT1 / rmConfig.stepSize);
			float opticalDepth = 0.f;
			for (int opticalDepthStep = 0; opticalDepthStep < opticalDepthSteps; opticalDepthStep++) {
				const float sampleVectorT = (opticalDepthStep + 0.5f) * rmConfig.stepSize;
				const Vector3 sampleVector = Vector3Add(lightRayOrigin, Vector3Scale(lightRayDir, sampleVectorT));

				opticalDepth += sampleFBMDensity(fbmConfig, sampleVector, sphereCenter, sphereRadius);
			}

			// transmittance through the volume from the lightT1 (point where the light ray intersects the sphere) and the light ray origin
			const float volTransmittance = exp(-opticalDepth * rmConfig.stepSize * extinctionCoeff);

			// color of the transmitted / attenuated light that hits the light ray origin
			const CustomColor transmittedLightSourceColor = light.getColor() * rmConfig.stepSize * volTransmittance * material.scatteringCoeff * density * hg;

			finalTransmittedLightSourceColor += (transmittedLightSourceColor * accumulatedStepVolTransmittance);
		}

		// Russian roulette method (optimization)
		// If the random number is large than 1/d break, else compensate for the lost light packets by * d
		if (accumulatedStepVolTransmittance < rmConfig.russianRouletteThreshold) {
			if (rng.uniform<float>(0.f, 1.f) > 1 / rmConfig.survivalWeight) {
				break;
			} else {
				accumulatedStepVolTransmittance *= rmConfig.survivalWeight;
			}
		}
	}

	// calculate the final bg color that is transmitted through the volume
	const CustomColor finalBGColor = bgColor * accumulatedStepVolTransmittance;

	return finalBGColor + finalTransmittedLightSourceColor;
}
