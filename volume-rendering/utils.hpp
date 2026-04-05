#pragma once

#include "raymath.h"
#include "custom-color.hpp"

CustomColor calculateBGColorThroughVolume(CustomColor bgColor, CustomColor volumeColor, float absorptionCoeff, float distance);
Vector3 computeRayDir(int col, int row, int screenW, int screenH);
