#pragma once

#include "raylib.h"
#include "shared/Types.h"

class SceneObject {
	public:
		virtual bool intersect(const Ray& ray, float& t0, float& t1) const = 0;
		virtual ~SceneObject() = default;
};
