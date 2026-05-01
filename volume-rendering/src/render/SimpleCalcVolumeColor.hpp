#pragma once
#include "utils/CustomColor.hpp"
#include "scene_objects/SceneObject.hpp"
#include "scene_objects/IVolumeObject.hpp"

CustomColor simpleCalcVolumeColor(CustomColor bgColor, const IVolumeObject& volume, float t0, float t1);
