#include "volume-rendering.h"
#include "sphere.hpp"

int main()
{
	constexpr int screenWidth = 800;
    constexpr int screenHeight = 400;
    constexpr int screenMidX = screenWidth * 0.5;
    constexpr int screenMidY = screenHeight * 0.5;
    Color bgColor = WHITE;
    std::unique_ptr<Sphere> sphere(new Sphere());
    Vector3 rayOrigin = { 0, 0, 0 };

    // 1. Initialize window
    InitWindow(screenWidth, screenHeight, "Volume Rendering");

    // 2. Generate single color background image
    Image image = GenImageColor(screenWidth, screenHeight, bgColor);

    // 3. Draw image
    for (int row = 0; row < screenHeight; row++) {
		for (int col = 0; col < screenWidth; col++) {
            Vector3 rayDir = computeRayDir(col, row, screenWidth, screenHeight);
            Ray ray = { rayOrigin, rayDir };
            Color finalColor;
            float t0, t1;

            if (sphere->intersect(ray, t0, t1)) {
                finalColor = sphere->computeVolumeColor(bgColor, t0, t1);
            } else {
                finalColor = bgColor;
            }
            
            ImageDrawPixel(&image, col, row, finalColor);
		}
    }

    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);
    Vector2 texturePos = { (screenMidX - texture.width * 0.5f), (screenMidY - texture.height * 0.5f) };

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(bgColor);
			DrawTextureV(texture, texturePos, RAYWHITE);
        EndDrawing();
    }
	return 0;
}
