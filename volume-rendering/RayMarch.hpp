#pragma once

#include "CustomColor.hpp"
#include "PointLight.hpp"
#include "Sphere.hpp"
#include "RandomNumberGenerator.hpp"

CustomColor rayMarchBackward(const CustomColor& bgColor, const float stepSize, const float t0, const float t1, const Ray& ray, PointLight* pLight, Sphere* sphere);
CustomColor rayMarchForward(const CustomColor& bgColor, const float stepSize, const float t0, const float t1, const Ray& ray, PointLight* pLight, Sphere* sphere, RandomNumberGenerator& rng);
