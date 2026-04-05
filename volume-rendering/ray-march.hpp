#pragma once

#include "custom-color.hpp"
#include "point-light.hpp"
#include "sphere.hpp"

CustomColor rayMarchBackward(const CustomColor& bgColor, const float stepSize, const float t0, const float t1, const Ray& ray, PointLight* pLight, Sphere* sphere);
CustomColor rayMarchForward(const CustomColor& bgColor, const float stepSize, const float t0, const float t1, const Ray& ray, PointLight* pLight, Sphere* sphere);
