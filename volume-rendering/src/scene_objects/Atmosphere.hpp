#pragma once

#include "raymath.h"
#include "utils/CustomColor.hpp"

struct AtmosphereConfig {
    float   earthRadius			= 6360e3f;
    float   atmosphereRadius	= 6420e3f;
    float   rayleighScaleHeight = 7994.f;
    float   mieScaleHeight		= 1200.f;
    Vector3 center				= { 0, 0, 0 };
    Vector3 sunDirection		= { 0, 0, -1 };

	int primaryRaySteps			= 16;
	int sunlightRaySteps		= 8;
	float sunIntensity			= 1000.f;
	float mieAsymmetryFactor	= 0.76f;

    // Physical scattering coefficients — const because they are measured values
	static const Vector3 rayleighScatteringCoeff;
	static const Vector3 mieScatteringCoeff;
};

// sunDirection: normalized
// scale height: altitude by which the atmosphere's density decreases by a factor of e
class Atmosphere {
	public:
		Atmosphere(const AtmosphereConfig& config);

		CustomColor calcIncidentLight(const Ray& ray, float t1) const;
		bool intersect(const Ray& ray, float& t0, float& t1, float radius) const;
		const AtmosphereConfig& getConfig() const;

	private:
		AtmosphereConfig config;
		float rayleighPhase(float cosTheta) const;
		float miePhase(float g, float cosTheta) const;
		CustomColor toneMap(CustomColor color) const;
		CustomColor toneMapExtended(CustomColor color, float maxWhite) const;
};