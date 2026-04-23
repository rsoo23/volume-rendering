#pragma once

#include "raymath.h"

class CustomCamera {
	public:
		CustomCamera();
		CustomCamera(Vector3 position);

		Vector3 getPosition() const;
		Vector3 computeRayDir(int row, int col, Vector2 screenDimension) const;

	private:
		Vector3 position;
};