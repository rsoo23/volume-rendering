#pragma once

#include "raymath.h"
#include "utils/CustomColor.hpp"

// sunDirection: normalized
// scale height: altitude by which the atmosphere's density decreases by a factor of e
class Atmosphere {
	public:
		Atmosphere();
		Atmosphere(Vector3 sunDirection, Vector3 center);

		CustomColor calcIncidentLight(const Ray& ray, float t1) const;
		bool intersect(const Ray& ray, float& t0, float& t1, float radius) const;

		Vector3 sunDirection;
		float earthRadius;
		float atmosphereRadius;
		Vector3 center;
		float rayleighScaleHeight;
		float mieScaleHeight;

		static const Vector3 rayleightScatteringCoefficients;
		static const Vector3 mieScatteringCoefficients;
	
	private:
		float rayleighPhase(float cosTheta) const;
		float miePhase(float g, float cosTheta) const;
		CustomColor toneMap(CustomColor color) const;
		CustomColor toneMapExtended(CustomColor color, float maxWhite) const;
};