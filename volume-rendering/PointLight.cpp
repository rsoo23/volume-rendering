#include "pch.h"
#include "PointLight.hpp"

PointLight::PointLight() : color(WHITE), position({ 0, 0, 0 }) {}

PointLight::PointLight(CustomColor color, Vector3 position) : color(color), position(position) {}
