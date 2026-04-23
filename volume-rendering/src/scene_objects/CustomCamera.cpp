#include "CustomCamera.hpp"

CustomCamera::CustomCamera() : position({ 0, 0, 0 }) {}

CustomCamera::CustomCamera(Vector3 position): position(position) {}

Vector3 CustomCamera::getPosition() const {
	return this->position;
}

Vector3 CustomCamera::computeRayDir(int row, int col, Vector2 screenDimension) const {
    // 1. Normalize to [0, 1]
    // +0.5f is for the ray the center of the pixel instead of the corner, preventing sampling artifacts
    float u = (col + 0.5f) / screenDimension.x;
    float v = (row + 0.5f) / screenDimension.y;

    // 2. Remap to [-1, 1]
    // flip v such that worldY is up instead of down
	//Screen space:        World space (NDC):
	//(0,0)──────(640,0)   (-1,+1)────(+1,+1)
	//  │                      │    
	//  │  (320,240)           │   (0,0)        
	//  │                      │    
	//(0,480)────(640,480) (-1,-1)────(+1,-1)
    u = u * 2.f - 1.f;
    v = 1.f - v * 2.f;

    // 3. Scale u by aspect ratio
    // stretch the horizontal rays back out to match the proportion of screen
    float aspectRatio = (float)screenDimension.x / screenDimension.y;
    u *= aspectRatio;

    // 4. Return normalized ray direction vector
    // -1.f means the ray is shooting into the screen
    return Vector3Normalize({ u, v, -1.f });
}
