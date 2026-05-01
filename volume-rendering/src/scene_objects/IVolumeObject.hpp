#pragma once
#include "SceneObject.hpp"
#include "shared/Types.h"
#include "raymath.h"

struct VolumeMaterial {
	CustomColor color;
	float absorptionCoeff;
	float scatteringCoeff;
	float density;
};

// All volume objects must support these — renderer calls through this interface
class IVolumeObject : public SceneObject {
	public:
		virtual bool           intersect(const Ray& ray, float& t0, float& t1) const = 0;
		virtual VolumeMaterial getMaterial() const = 0;
		virtual Vector3        getCenter() const = 0;
		virtual float          getRadius() const = 0;
};