#pragma once
#pragma once

#include "raymath.h"
#include "custom-color.hpp"

class Sphere {
	public:
		Sphere();
		Sphere(float absorptionCoeff, float scatteringCoeff, float density, CustomColor color, Vector3 center, float radius);

		bool intersect(const Ray& ray, float& t0, float& t1) const;
		CustomColor computeVolumeColor(const CustomColor& bgColor, float t0, float t1) const;

		float absorptionCoeff;
		float scatteringCoeff;
		float density;
		CustomColor color;
		Vector3 center;
		float radius;
};