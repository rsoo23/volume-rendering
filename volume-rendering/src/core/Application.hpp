#pragma once

#include "raymath.h"
#include <functional>
#include "utils/CustomColor.hpp"

class Application {
	public:
		Application();
		Application(Vector2 screenDimension, const char* title);
		~Application();

		void run(const std::function<void()>& draw) const;
		void drawTexture(CustomColor bgColor, Texture2D texture, Vector2 texturePos, CustomColor textureTint) const;

	private:
		int screenWidth;
		int screenHeight;
		const char* title;
};