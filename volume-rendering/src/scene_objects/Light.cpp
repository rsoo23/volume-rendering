#include "pch.h"
#include "Light.hpp"

Light::Light() : color(WHITE), direction({ 0, 0, 0 }) {}

Light::Light(CustomColor color, Vector3 direction) : color(color), direction(direction) {}

CustomColor Light::getColor() const {
	return this->color;
}

Vector3 Light::getDirection() const {
	return this->direction;
}
