#pragma once

#include "raymath.h"
#include "SceneObject.hpp"
#include "utils/CustomColor.hpp"

// Equation: ax + by + cz = d
// - d: signed distance (distance along normal)
// - p0: point on plane (derived from normal and d)
class Plane: public SceneObject {
	public:
		Plane();
		Plane(CustomColor color, Vector3 normal, float d);

		bool intersect(const Ray& ray, float& t) const;
		CustomColor getColor() const;
		Vector3 getNormal() const;

	private:
		CustomColor color;
		Vector3 normal;
		float d;
		Vector3 p0;
};