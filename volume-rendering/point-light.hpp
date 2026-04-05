#pragma once

#include "raymath.h"
#include "custom-color.hpp"

class PointLight {
	public:
		PointLight();
		PointLight(CustomColor color, Vector3 position);

		CustomColor color;
		Vector3 position;
};