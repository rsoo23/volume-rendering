#include "pch.h"
#include "custom-color.hpp"
#include "sphere.hpp"
#include "point-light.hpp"
#include "utils.hpp"
#include "ray-march.hpp"

int main()
{
	constexpr int screenWidth = 800;
    constexpr int screenHeight = 400;
    constexpr int screenMidX = screenWidth * 0.5;
    constexpr int screenMidY = screenHeight * 0.5;

    CustomColor bgColor(WHITE);

    constexpr float sphereDensity = 0.8;
    CustomColor sphereColor(BLUE);
    const Vector3 spherePos = { 0, 0, -20 };
    constexpr float sphereRadius = 15.f;

    CustomColor pointLightColor(PURPLE);
    const Vector3 pointLightPos = { 0, 15, -20 };

    Vector3 rayOrigin = { 0, 0, 0 };

    std::unique_ptr<Sphere> sphere(new Sphere(sphereDensity, sphereColor, spherePos, sphereRadius));
    std::unique_ptr<PointLight> pLight(new PointLight(pointLightColor, pointLightPos));

    const float stepSize = 0.01f;

    // 1. Initialize window
    InitWindow(screenWidth, screenHeight, "Volume Rendering");

    // 2. Generate single color background image
    Image image = GenImageColor(screenWidth, screenHeight, bgColor.getColor());

    // 3. Draw image
    for (int row = 0; row < screenHeight; row++) {
		for (int col = 0; col < screenWidth; col++) {
            Vector3 rayDir = computeRayDir(col, row, screenWidth, screenHeight);
            Ray ray = { rayOrigin, rayDir };
            CustomColor finalColor;
            float t0, t1;

            if (sphere->intersect(ray, t0, t1)) {
                //finalColor = sphere->computeVolumeColor(bgColor, t0, t1);
                finalColor = rayMarchBackward(bgColor, stepSize, t0, t1, ray, pLight.get(), sphere.get());
                //finalColor = rayMarchForward(bgColor, stepSize, t0, t1, ray, pLight.get(), sphere.get());
            } else {
                finalColor = bgColor;
            }
            
            ImageDrawPixel(&image, col, row, finalColor.getColor());
		}
    }

    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    Vector2 texturePos = { (screenMidX - texture.width * 0.5f), (screenMidY - texture.height * 0.5f) };

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(bgColor.getColor());
			DrawTextureV(texture, texturePos, RAYWHITE);
        EndDrawing();
    }
	return 0;
}
