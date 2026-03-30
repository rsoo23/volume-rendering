// volume-rendering.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include "raylib.h"
#include "raymath.h"

Color calculateBGColorThroughVolume(Color bgColor, Color volumeColor, float density, float distance);
Vector3 computeRayDir(int col, int row, int screenW, int screenH);
