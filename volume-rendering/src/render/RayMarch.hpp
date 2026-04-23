#pragma once

#include "utils/CustomColor.hpp"
#include "math/RandomNumberGenerator.hpp"
#include "scene_objects/Light.hpp"
#include "scene_objects/Sphere.hpp"

CustomColor rayMarchBackward(CustomColor bgColor, const Light& light, const Sphere& sphere, float asymmetryFactor, float stepSize, float t0, float t1, const Ray& ray);
CustomColor rayMarchForward(CustomColor bgColor, const Light& light, const Sphere& sphere, float asymmetryFactor, float stepSize, float survivalWeight, float t0, float t1, const Ray& ray);
