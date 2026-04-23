#pragma once
#include "utils/CustomColor.hpp"
#include "scene_objects/SceneObject.hpp"
#include "scene_objects/Sphere.hpp"

CustomColor simpleComputeVolumeColor(CustomColor bgColor, const Sphere& sphere, float t0, float t1);
