#include "pch.h"
#include "Application.hpp"

Application::Application():
	screenWidth(800),
	screenHeight(400),
	title("Volume Rendering")
{
	InitWindow(screenWidth, screenHeight, title);
};

Application::Application(Vector2 screenDimension, const char* title):
	screenWidth(screenDimension.x),
	screenHeight(screenDimension.y),
	title(title)
{
	InitWindow(screenWidth, screenHeight, title);
}

void Application::run(const std::function<void()>& draw) const {
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
		draw();
    }
}

void Application::drawTexture(CustomColor bgColor, Texture2D texture, Vector2 texturePos, CustomColor textureTint) const {
	BeginDrawing();
		ClearBackground(bgColor.getColor());
		DrawTextureV(texture, texturePos, textureTint.getColor());
	EndDrawing();
}

Application::~Application() {};
