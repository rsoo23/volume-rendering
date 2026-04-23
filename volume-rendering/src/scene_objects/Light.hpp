#pragma once

#include "raymath.h"
#include "utils/CustomColor.hpp"

class Light {
	public:
		Light();
		Light(CustomColor color, Vector3 direction);

		CustomColor getColor() const;
		Vector3 getDirection() const;

	private:
		CustomColor color;
		Vector3 direction;
};