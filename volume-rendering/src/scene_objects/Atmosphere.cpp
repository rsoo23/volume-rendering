#include "pch.h"
#include "Atmosphere.hpp"

Atmosphere::Atmosphere(const AtmosphereConfig& config): config(config) {}

const Vector3 AtmosphereConfig::rayleighScatteringCoeff	= { 3.8e-6f, 13.5e-6f, 33.1e-6f };
const Vector3 AtmosphereConfig::mieScatteringCoeff		= { 21e-6f, 21e-6f, 21e-6f }; 

CustomColor Atmosphere::calcIncidentLight(const Ray& ray, float t1) const {
	const float stepSize = t1 / this->config.primaryRaySteps;
	Vector3 finalRayleighTransmittanceSum({ 0, 0, 0 }), finalMieTransmittanceSum({ 0, 0, 0 });
	float rayleighPrimaryRayTransmittance = 0.f, miePrimaryRayTransmittance = 0.f;
	CustomColor finalColor;

	for (int step = 0; step < this->config.primaryRaySteps; step++) {
		const float sampleT = (step + 0.5f) * stepSize;
		const Vector3 samplePos = Vector3Add(ray.position, Vector3Scale(ray.direction, sampleT));
		const Ray sunlightRay = { samplePos, this->config.sunDirection };
		float sunlightT0, sunlightT1;

		const float altitude = Vector3Length(samplePos) - this->config.earthRadius;

		// transmittance per step
		const float stepRayleighTransmittance = exp(-altitude / this->config.rayleighScaleHeight) * stepSize;
		const float stepMieTransmittance = exp(-altitude / this->config.mieScaleHeight) * stepSize;

		rayleighPrimaryRayTransmittance += stepRayleighTransmittance;
		miePrimaryRayTransmittance += stepMieTransmittance;

		float rayleighSunlightTransmittance = 0.f, mieSunlightTransmittance = 0.f;

		// sample sunlight ray
		if (this->intersect(sunlightRay, sunlightT0, sunlightT1, this->config.atmosphereRadius)) {
			const float sunlightStepSize = sunlightT1 / this->config.sunlightRaySteps;
			for (int sunlightStep = 0; sunlightStep < this->config.sunlightRaySteps; sunlightStep++) {
				const float sunlightSampleT = (sunlightStep + 0.5f) * sunlightStepSize;
				const Vector3 sunlightSamplePos = Vector3Add(samplePos, Vector3Scale(this->config.sunDirection, sunlightSampleT));

				const float sunlightSampleAltitude = Vector3Length(sunlightSamplePos) - this->config.earthRadius;

				rayleighSunlightTransmittance += exp(-sunlightSampleAltitude / this->config.rayleighScaleHeight) * stepSize;
				mieSunlightTransmittance += exp(-sunlightSampleAltitude / this->config.mieScaleHeight) * stepSize;
			}

			// accumulate transmittance
			Vector3 scatteredRayleigh = Vector3Scale(AtmosphereConfig::rayleighScatteringCoeff, (rayleighPrimaryRayTransmittance + rayleighSunlightTransmittance));
			Vector3 scatteredMie = Vector3Scale(AtmosphereConfig::mieScatteringCoeff, 1.1f * (miePrimaryRayTransmittance + mieSunlightTransmittance));
			Vector3 tau = Vector3Add(scatteredRayleigh, scatteredMie);
			Vector3 attenuation = { exp(-tau.x), exp(-tau.y), exp(-tau.z) };

			finalRayleighTransmittanceSum = Vector3Add(Vector3Scale(attenuation, stepRayleighTransmittance), finalRayleighTransmittanceSum);
			finalMieTransmittanceSum = Vector3Add(Vector3Scale(attenuation, stepMieTransmittance), finalMieTransmittanceSum);
		}
	}
	const float cosTheta = Vector3DotProduct(this->config.sunDirection, Vector3Scale(ray.direction, -1));
	const float rayleighPhaseVal = rayleighPhase(cosTheta);
	const float miePhaseVal = miePhase(this->config.mieAsymmetryFactor, cosTheta);

	Vector3 finalVector = (
		Vector3Add(
			Vector3Scale(Vector3Multiply(finalRayleighTransmittanceSum, AtmosphereConfig::rayleighScatteringCoeff), rayleighPhaseVal),
			Vector3Scale(Vector3Multiply(finalMieTransmittanceSum, AtmosphereConfig::mieScatteringCoeff), miePhaseVal)
		)
	);
	finalColor = { finalVector.x, finalVector.y, finalVector.z, 1 };
	return this->toneMap(finalColor * this->config.sunIntensity);
}

bool Atmosphere::intersect(const Ray& ray, float& t0, float& t1, float radius) const {
	Vector3 oc = Vector3Subtract(ray.position, this->config.center);

	float a = Vector3DotProduct(ray.direction, ray.direction);
	float b = 2.f * Vector3DotProduct(ray.direction, oc);
	float c = Vector3DotProduct(oc, oc) - radius * radius;

	float discriminant = b * b - 4.f * a * c;

	if (discriminant < 0) return false;
	else if (discriminant == 0) t0 = t1 = -0.5f * b / a; 
	else {
		float q = (b > 0) ?
			-0.5f * (b + sqrt(discriminant)) :
            -0.5f * (b - sqrt(discriminant));
        t0 = q / a;
        t1 = c / q;
    }

	// guarantees nearest point is always t0
    if (t0 > t1) std::swap(t0, t1);
    
    return true;
}
const AtmosphereConfig& Atmosphere::getConfig() const {
	return this->config;
}

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
