#include "pch.h"
#include "point-light.hpp"
#include "sphere.hpp"

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

	for (int step = 0; step < steps; step++) {
		// t value of the light ray origin (steps start from t1 and go backward)
		const float lightRayOriginT = t1 - (step * stepSize) - (0.5f * stepSize);
		const Vector3 lightRayOrigin = Vector3Add(ray.position, Vector3Scale(ray.direction, lightRayOriginT));
		const Vector3 lightRayDir = Vector3Normalize(Vector3Subtract(pLight->position, lightRayOrigin));
		const Ray lightRay = { lightRayOrigin, lightRayDir };
		float lightT0, lightT1;

		// in-scattering
		if (sphere->intersect(lightRay, lightT0, lightT1)) {
			// transmittance through the volume from the lightT1 (point where the light ray intersects the sphere) and the light ray origin
			const float volTransmittance = exp(-lightT1 * extinctionCoeff);
			// color of the transmitted / attenuated light that hits the light ray origin
			const CustomColor transmittedLightSourceColor = pLight->color * stepSize * volTransmittance * sphere->scatteringCoeff * sphere->density;

			finalTransmittedLightSourceColor += transmittedLightSourceColor;
		}

		finalTransmittedLightSourceColor *= stepVolTransmittance;
	}
	return finalBGColor + finalTransmittedLightSourceColor;
}

CustomColor rayMarchForward(const CustomColor& bgColor, const float stepSize, const float t0, const float t1, const Ray& ray, PointLight* pLight, Sphere* sphere) {
	const int steps = (int)((t1 - t0) / stepSize);
	CustomColor finalTransmittedLightSourceColor;

	const float extinctionCoeff = (sphere->absorptionCoeff + sphere->scatteringCoeff) * sphere->density;

	// transmittance per step in the volume
	const float stepVolTransmittance = exp(-stepSize * extinctionCoeff);
	float accumulatedStepVolTransmittance = 1.f;

	for (int step = 0; step < steps; step++) {
		// t value of the light ray origin (steps start from t1 and go forward)
		const float lightRayOriginT = t0 + (step * stepSize) + (0.5f * stepSize);
		const Vector3 lightRayOrigin = Vector3Add(ray.position, Vector3Scale(ray.direction, lightRayOriginT));
		const Vector3 lightRayDir = Vector3Normalize(Vector3Subtract(pLight->position, lightRayOrigin));
		const Ray lightRay = { lightRayOrigin, lightRayDir };
		float lightT0, lightT1;

		accumulatedStepVolTransmittance *= stepVolTransmittance;

		// in-scattering
		if (sphere->intersect(lightRay, lightT0, lightT1)) {
			// transmittance through the volume from the lightT1 (point where the light ray intersects the sphere) and the light ray origin
			const float volTransmittance = exp(-lightT1 * extinctionCoeff);
			// color of the transmitted / attenuated light that hits the light ray origin
			const CustomColor transmittedLightSourceColor = pLight->color * stepSize * volTransmittance * sphere->scatteringCoeff * sphere->density;

			finalTransmittedLightSourceColor += (transmittedLightSourceColor * accumulatedStepVolTransmittance);
		}
	}

	// calculate the final bg color that is transmitted through the volume
	const CustomColor finalBGColor = bgColor * accumulatedStepVolTransmittance;

	return finalBGColor + finalTransmittedLightSourceColor;
}
