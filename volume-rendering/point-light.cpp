#include "pch.h"
#include "point-light.hpp"

PointLight::PointLight() : color(WHITE), position({ 0, 0, 0 }) {}

PointLight::PointLight(CustomColor color, Vector3 position) : color(color), position(position) {}
