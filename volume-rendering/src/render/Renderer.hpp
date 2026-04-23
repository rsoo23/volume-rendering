#pragma once

#include "raylib.h"
#include "utils/CustomColor.hpp"
#include "scene_objects/CustomCamera.hpp"
#include "scene/Scene.hpp"
#include "shared/Types.h"

class Renderer {
	public:
		Renderer();
		Texture2D render(Vector2 screenDimension, const Scene& scene, const CustomCamera& cam, AlgType algType);
};