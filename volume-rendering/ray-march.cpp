#include "pch.h"
#include "point-light.hpp"
#include "sphere.hpp"

// transmitted light source is caused by the in-scattering of light

CustomColor rayMarchBackward(const CustomColor& bgColor, const float stepSize, const float t0, const float t1, const Ray& ray, PointLight* pLight, Sphere* sphere) {
	const int steps = (int)((t1 - t0) / stepSize);
	CustomColor finalTransmittedLightSourceColor;

	// transmittance per step in the volume
	const float stepVolTransmittance = exp(-stepSize * sphere->density);

	// precalculate the final bg color that is transmitted through the volume
	const CustomColor finalBGColor = bgColor * pow(stepVolTransmittance, steps);

	for (int step = 0; step < steps; step++) {
		// t value of the light ray origin (steps start from t1 and go backward)
		const float lightRayOriginT = t1 - (step * stepSize) - (0.5f * stepSize);
		const Vector3 lightRayOrigin = Vector3Add(ray.position, Vector3Scale(ray.direction, lightRayOriginT));
		const Vector3 lightRayDir = Vector3Normalize(Vector3Subtract(pLight->position, lightRayOrigin));
		const Ray lightRay = { lightRayOrigin, lightRayDir };
		float lightT0, lightT1;

		if (sphere->intersect(lightRay, lightT0, lightT1)) {
			// transmittance through the volume from the lightT1 (point where the light ray intersects the sphere) and the light ray origin
			const float volTransmittance = exp(-lightT1 * sphere->density);
			// color of the transmitted / attenuated light that hits the light ray origin
			const CustomColor transmittedLightSourceColor = pLight->color * stepSize * volTransmittance;

			finalTransmittedLightSourceColor += transmittedLightSourceColor;
		}

		finalTransmittedLightSourceColor *= stepVolTransmittance;
	}
	return finalBGColor + finalTransmittedLightSourceColor;
}

CustomColor rayMarchForward(const CustomColor& bgColor, const float stepSize, const float t0, const float t1, const Ray& ray, PointLight* pLight, Sphere* sphere) {
	const int steps = (int)((t1 - t0) / stepSize);
	CustomColor finalTransmittedLightSourceColor;

	// transmittance per step in the volume
	const float stepVolTransmittance = exp(-stepSize * sphere->density);
	float accumulatedStepVolTransmittance = 1.f;

	for (int step = 0; step < steps; step++) {
		// t value of the light ray origin (steps start from t1 and go forward)
		const float lightRayOriginT = t0 + (step * stepSize) + (0.5f * stepSize);
		const Vector3 lightRayOrigin = Vector3Add(ray.position, Vector3Scale(ray.direction, lightRayOriginT));
		const Vector3 lightRayDir = Vector3Normalize(Vector3Subtract(pLight->position, lightRayOrigin));
		const Ray lightRay = { lightRayOrigin, lightRayDir };
		float lightT0, lightT1;

		accumulatedStepVolTransmittance *= stepVolTransmittance;

		if (sphere->intersect(lightRay, lightT0, lightT1)) {
			// transmittance through the volume from the lightT1 (point where the light ray intersects the sphere) and the light ray origin
			const float volTransmittance = exp(-lightT1 * sphere->density);
			// color of the transmitted / attenuated light that hits the light ray origin
			const CustomColor transmittedLightSourceColor = pLight->color * stepSize * volTransmittance;

			finalTransmittedLightSourceColor += (transmittedLightSourceColor * accumulatedStepVolTransmittance);
		}
	}

	// calculate the final bg color that is transmitted through the volume
	const CustomColor finalBGColor = bgColor * accumulatedStepVolTransmittance;

	return finalBGColor + finalTransmittedLightSourceColor;
}
