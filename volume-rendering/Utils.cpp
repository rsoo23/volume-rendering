#include "pch.h"
#include "Utils.hpp"

// absorptionCoeff: absorption coefficient of volume
// distance: thickness of volume
CustomColor calculateBGColorThroughVolume(CustomColor bgColor, CustomColor volumeColor, float absorptionCoeff, float distance) {
    float transmittance = exp(-distance * absorptionCoeff);
    CustomColor newColor = bgColor * transmittance + volumeColor * (1 - transmittance);
    return newColor;
}

Vector3 computeRayDir(int col, int row, int screenW, int screenH) {
    // 1. normalize to [0, 1]
    // +0.5f is for the ray the center of the pixel instead of the corner, preventing sampling artifacts
    float u = (col + 0.5f) / screenW;
    float v = (row + 0.5f) / screenH;

    // 2. remap to [-1, 1]
    // flip v such that worldY is up instead of down
	//Screen space:        World space (NDC):
	//(0,0)──────(640,0)   (-1,+1)────(+1,+1)
	//  │                      │    
	//  │  (320,240)           │   (0,0)        
	//  │                      │    
	//(0,480)────(640,480) (-1,-1)────(+1,-1)
    u = u * 2.f - 1.f;
    v = 1.f - v * 2.f;

    // 3. scale u by aspect ratio
    // stretch the horizontal rays back out to match the proportion of screen
    float aspectRatio = (float)screenW / screenH;
    u *= aspectRatio;

    // 4. return normalized ray direction vector
    // -1.f means shooting into the screen
    return Vector3Normalize({ u, v, -1.f });
}