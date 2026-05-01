#pragma once

#include "raylib.h"
#include "utils/CustomColor.hpp"
#include "scene_objects/CustomCamera.hpp"
#include "scene/Scene.hpp"
#include "shared/Types.h"
#include "render/RayMarch.hpp"

class Renderer {
	public:
		Renderer();
		Texture2D render(
			const RayMarchConfig& rmConfig,
			const FBMConfig& fbmConfig,
			const Vector2 screenDimension,
			const Scene& scene,
			const CustomCamera& cam,
			AlgType algType
		);
};