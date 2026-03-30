#pragma once

#include "volume-rendering.h"

class Sphere {
	public:
		bool intersect(const Ray& ray, float& t0, float& t1);
		Color computeVolumeColor(const Color& bgColor, const float t0, const float t1);

		float density{ 0.03 };
		Color color{ PURPLE };
		Vector3 center{ 0, 0, -10 };
		float radius{ 5 };
};