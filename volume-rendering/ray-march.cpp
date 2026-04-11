#include "pch.h"
#include "point-light.hpp"
#include "sphere.hpp"
#include "random-number-generator.hpp"

// 1 / (4 * PI): represents the phase function where all out-scattered directions are equally likely to be chosen
// g (-1 <= g <= 1): the asymmetry factor controls whether light is out-scattered in the forward / backward direction
// - -1 < g < 0: scattered backward
// - g = 0: equals to 1 / (4 * PI)
// - 0 < g < 1: scattered forward
// cosTheta: cosine of angle between light direction (object to light) and view direction (object to camera)
static float henyeyGreensteinPhase(const float g, const float cosTheta) {
	float denom = 1 + g * g - 2 * g * cosTheta;
	return 1 / (4 * PI) * (1 - g * g) / (denom * sqrtf(denom));
}

// in-scattering: 
// - when light that is not traveling to the eye scatters against a particle, sending a portion of light to the eye
// - gather all light from all directions that isn't meant for you and return some in your direction

// out-scattering:
// - when light that is traveling to the eye scatters against a particle, sending a portion of light to the eye
// - light is traveling towards you but some light is scattered in random directions, some will be in your direction

CustomColor rayMarchBackward(const CustomColor& bgColor, const float stepSize, const float t0, const float t1, const Ray& ray, PointLight* pLight, Sphere* sphere) {
	const int steps = (int)((t1 - t0) / stepSize);
	CustomColor finalTransmittedLightSourceColor;

	const float extinctionCoeff = (sphere->absorptionCoeff + sphere->scatteringCoeff) * sphere->density;

	// transmittance per step in the volume
	const float stepVolTransmittance = exp(-stepSize * extinctionCoeff);

	// precalculate the final bg color that is transmitted through the volume
	const CustomColor finalBGColor = bgColor * pow(stepVolTransmittance, steps);

	// anisotropic / asymmetry factor for Henyey-Greenstein phase function
	const float g = 0.5;

	const Vector3 lightRayDir = { 0, 0, 1 };

	for (int step = 0; step < steps; step++) {
		// t value of the light ray origin (steps start from t1 and go backward)
		const float lightRayOriginT = t1 - (step * stepSize) - (0.5f * stepSize);
		const Vector3 lightRayOrigin = Vector3Add(ray.position, Vector3Scale(ray.direction, lightRayOriginT));
		const Ray lightRay = { lightRayOrigin, lightRayDir };
		float lightT0, lightT1;

		// in-scattering
		if (sphere->intersect(lightRay, lightT0, lightT1)) {
			const float cosTheta = Vector3DotProduct(lightRay.direction, Vector3Scale(ray.direction, -1));
			const float hg = henyeyGreensteinPhase(g, cosTheta);
			// transmittance through the volume from the lightT1 (point where the light ray intersects the sphere) and the light ray origin
			const float volTransmittance = exp(-lightT1 * extinctionCoeff);
			// color of the transmitted / attenuated light that hits the light ray origin
			const CustomColor transmittedLightSourceColor = pLight->color * stepSize * volTransmittance * sphere->scatteringCoeff * sphere->density * hg;

			finalTransmittedLightSourceColor += transmittedLightSourceColor;
		}

		finalTransmittedLightSourceColor *= stepVolTransmittance;
	}
	return finalBGColor + finalTransmittedLightSourceColor;
}

CustomColor rayMarchForward(const CustomColor& bgColor, const float stepSize, const float t0, const float t1, const Ray& ray, PointLight* pLight, Sphere* sphere, RandomNumberGenerator& rng) {
	const int steps = (int)((t1 - t0) / stepSize);
	CustomColor finalTransmittedLightSourceColor;

	const float extinctionCoeff = (sphere->absorptionCoeff + sphere->scatteringCoeff) * sphere->density;

	// transmittance per step in the volume
	const float stepVolTransmittance = exp(-stepSize * extinctionCoeff);
	float accumulatedStepVolTransmittance = 1.f;

	// anisotropic asymmetry factor for Henyey-Greenstein phase function
	const float g = -0.80f;

	const Vector3 lightRayDir = { 0, 0, 1 };

	for (int step = 0; step < steps; step++) {
		// t value of the light ray origin (steps start from t1 and go forward)
		const float lightRayOriginT = t0 + (step + rng.uniform<float>(0.f, 1.f)) * stepSize;
		const Vector3 lightRayOrigin = Vector3Add(ray.position, Vector3Scale(ray.direction, lightRayOriginT));
		const Ray lightRay = { lightRayOrigin, lightRayDir };
		float lightT0, lightT1;

		accumulatedStepVolTransmittance *= stepVolTransmittance;

		// in-scattering
		if (sphere->intersect(lightRay, lightT0, lightT1)) {
			const float cosTheta = Vector3DotProduct(lightRayDir, Vector3Scale(ray.direction, -1));
			const float hg = henyeyGreensteinPhase(g, cosTheta);
			// transmittance through the volume from the lightT1 (point where the light ray intersects the sphere) and the light ray origin
			const float volTransmittance = exp(-lightT1 * extinctionCoeff);
			// color of the transmitted / attenuated light that hits the light ray origin
			const CustomColor transmittedLightSourceColor = pLight->color * stepSize * volTransmittance * sphere->scatteringCoeff * sphere->density * hg;

			finalTransmittedLightSourceColor += (transmittedLightSourceColor * accumulatedStepVolTransmittance);
		}
	}

	// calculate the final bg color that is transmitted through the volume
	const CustomColor finalBGColor = bgColor * accumulatedStepVolTransmittance;

	return finalBGColor + finalTransmittedLightSourceColor;
}
