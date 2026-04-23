#pragma once

#include "raymath.h"
#include "SceneObject.hpp"
#include "shared/Types.h"
#include "utils/CustomColor.hpp"

class Sphere: public SceneObject {
	public:
		Sphere();
		Sphere(CustomColor color, float absorptionCoeff, float scatteringCoeff, float density, Vector3 center, float radius);

		bool intersect(const Ray& ray, float& t0, float& t1) const;
		VolumeMaterial getMaterial() const;
		Vector3 getCenter() const;
		float getRadius() const;

	private:
		VolumeMaterial material;
		Vector3 center;
		float radius;
};