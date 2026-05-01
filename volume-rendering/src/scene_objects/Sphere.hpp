#pragma once

#include "raymath.h"
#include "SceneObject.hpp"
#include "shared/Types.h"
#include "utils/CustomColor.hpp"
#include "IVolumeObject.hpp"

class Sphere: public IVolumeObject {
	public:
		Sphere();
		Sphere(const VolumeMaterial& mat, Vector3 center, float radius);

		bool intersect(const Ray& ray, float& t0, float& t1) const override;
		VolumeMaterial getMaterial() const override;
		Vector3 getCenter() const;
		float getRadius() const;

	private:
		VolumeMaterial material;
		Vector3 center;
		float radius;
};