#include "pch.h"
#include "Atmosphere.hpp"

Atmosphere::Atmosphere():
	sunDirection(Vector3Normalize({ 0, 0, -1 })),
	earthRadius(6360e3),
	atmosphereRadius(6420e3),
	center({ 0, 0, 0 }),
	rayleighScaleHeight(7994),
	mieScaleHeight(1200) {}

Atmosphere::Atmosphere(Vector3 sunDirection, Vector3 center):
	sunDirection(Vector3Normalize(sunDirection)),
	earthRadius(6360e3),
	atmosphereRadius(6420e3),
	center(center),
	rayleighScaleHeight(7994),
	mieScaleHeight(1200) {}

const Vector3 Atmosphere::rayleightScatteringCoefficients({ 3.8e-6f, 13.5e-6f, 33.1e-6f });
const Vector3 Atmosphere::mieScatteringCoefficients({ 21e-6f, 21e-6f, 21e-6f }); 

CustomColor Atmosphere::calcIncidentLight(const Ray& ray, float t0, float t1) const {
	const float steps = 5;
	const int stepSize = t1 / steps;
	const float sunIntensity = 750.f;
	Vector3 finalRayleighTransmittanceSum({ 0, 0, 0 }), finalMieTransmittanceSum({ 0, 0, 0 });
	float rayleighPrimaryRayTransmittance = 0, miePrimaryRayTransmittance = 0;
	CustomColor finalColor;

	// Mie Phase Function g
	float asymmetryFactor = 0.76f;

	for (int step = 0; step < steps; step++) {
		const float sampleT = (step + 0.5) * stepSize;
		const Vector3 samplePos = Vector3Add(ray.position, Vector3Scale(ray.direction, sampleT));
		const Ray sunlightRay = { samplePos, this->sunDirection };
		float sunlightT0, sunlightT1;

		const float altitude = Vector3Length(samplePos) - this->earthRadius;

		// transmittance per step
		const float stepRayleighTransmittance = exp(-altitude / this->rayleighScaleHeight) * stepSize;
		const float stepMieTransmittance = exp(-altitude / this->mieScaleHeight) * stepSize;

		rayleighPrimaryRayTransmittance += stepRayleighTransmittance;
		miePrimaryRayTransmittance += stepMieTransmittance;

		float rayleighSunlightTransmittance = 0, mieSunlightTransmittance = 0; 

		// sample sunlight ray
		if (this->intersect(sunlightRay, sunlightT0, sunlightT1)) {
			for (int sunlightStep = 0; sunlightStep < steps; sunlightStep++) {
				const float sunlightSampleT = (sunlightStep + 0.5) * stepSize;
				const Vector3 sunlightSamplePos = Vector3Add(samplePos, Vector3Scale(this->sunDirection, sunlightSampleT));

				const float sunlightSampleAltitude = Vector3Length(sunlightSamplePos) - this->earthRadius;

				rayleighSunlightTransmittance += exp(-sunlightSampleAltitude / this->rayleighScaleHeight) * stepSize;
				mieSunlightTransmittance += exp(-sunlightSampleAltitude / this->mieScaleHeight) * stepSize;
			}

			// accumulate transmittance
			Vector3 scatteredRayleigh = Vector3Scale(this->rayleightScatteringCoefficients, (rayleighPrimaryRayTransmittance + rayleighSunlightTransmittance));
			Vector3 scatteredMie = Vector3Scale(this->mieScatteringCoefficients, 1.1 * (miePrimaryRayTransmittance + mieSunlightTransmittance));
			Vector3 tau = Vector3Add(scatteredRayleigh, scatteredMie);
			Vector3 attenuation = { exp(-tau.x), exp(-tau.y), exp(-tau.z) };

			finalRayleighTransmittanceSum = Vector3Add(Vector3Scale(attenuation, stepRayleighTransmittance), finalRayleighTransmittanceSum);
			finalMieTransmittanceSum = Vector3Add(Vector3Scale(attenuation, stepMieTransmittance), finalMieTransmittanceSum);
		}
	}
	const float cosTheta = Vector3DotProduct(this->sunDirection, Vector3Scale(ray.direction, -1));
	const float rayleighPhaseVal = rayleighPhase(cosTheta);
	const float miePhaseVal = miePhase(asymmetryFactor, cosTheta);

	Vector3 finalVector = (
		Vector3Add(
			Vector3Scale(Vector3Multiply(finalRayleighTransmittanceSum, this->rayleightScatteringCoefficients), rayleighPhaseVal),
			Vector3Scale(Vector3Multiply(finalMieTransmittanceSum, this->mieScatteringCoefficients), miePhaseVal)
		)
	);
	finalColor = { finalVector.x, finalVector.y, finalVector.z, 1 };
	return this->toneMapExtended(finalColor * sunIntensity, 1);
}

bool Atmosphere::intersect(const Ray& ray, float& t0, float& t1) const {
	Vector3 oc = Vector3Subtract(ray.position, this->center);

	float a = Vector3DotProduct(ray.direction, ray.direction);
	float b = 2 * Vector3DotProduct(ray.direction, oc);
	float c = Vector3DotProduct(oc, oc) - this->atmosphereRadius * this->atmosphereRadius;

	float discriminant = b * b - 4 * a * c;

	if (discriminant < 0) return false;
	else if (discriminant == 0) t0 = t1 = -0.5 * b / a; 
	else {
		float q = (b > 0) ?
			-0.5 * (b + sqrt(discriminant)) :
            -0.5 * (b - sqrt(discriminant));
        t0 = q / a;
        t1 = c / q;
    }

	// guarantees nearest point is always t0
    if (t0 > t1) std::swap(t0, t1);
    
    return true;
};

float Atmosphere::rayleighPhase(float cosTheta) const {
	return 3 / (16 * PI) * (1 + cosTheta * cosTheta);
}

float Atmosphere::miePhase(float g, float cosTheta) const {
	float denom = 1 + g * g - 2 * g * cosTheta;
	return 3 / (8 * PI) * (1 - g * g) * (1 + cosTheta * cosTheta) / ((2 + g * g) * (denom * sqrtf(denom)));
}

// Reinhard tone mapping
CustomColor Atmosphere::toneMap(CustomColor color) const {
	CustomColor denom = color + CustomColor(1, 1, 1, 1);
	return color / denom;
}

// Extended Reinhard tone mapping
// maxWhite: biggest radiance will get mapped to this value
CustomColor Atmosphere::toneMapExtended(CustomColor color, float maxWhite) const {
	float squareMaxWhite = maxWhite * maxWhite;
	CustomColor numerator = color * (CustomColor(1, 1, 1, 1) + color / CustomColor(squareMaxWhite, squareMaxWhite, squareMaxWhite, 1));
	return numerator / (CustomColor(1, 1, 1, 1) + color);
}
