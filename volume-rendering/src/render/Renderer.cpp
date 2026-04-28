#include "pch.h"
#include "Renderer.hpp"
#include "math/RandomNumberGenerator.hpp"
#include "render/RayMarch.hpp"
#include "scene_objects/Sphere.hpp"
#include "render/SimpleCalcVolumeColor.hpp"
#include "scene_objects/Plane.hpp"
#include "scene_objects/Atmosphere.hpp"

Renderer::Renderer() {}

Texture2D Renderer::render(Vector2 screenDimension, const Scene& scene, const CustomCamera& cam, AlgType algType) {
	const Atmosphere& atmosphere = scene.getAtmosphere();
	const Light& light = *(scene.getLights())[0];
	Sphere* sphere = dynamic_cast<Sphere *>((scene.getSceneObjects())[0].get());
	float asymmetryFactor = 0.2f;
	float stepSize = 0.1f;
	int survivalWeight = 10;

    // 1. Select algorithm
    std::function<CustomColor(CustomColor bgColor, const Ray& ray, float t0, float t1)> renderAlg;
    switch (algType) {
		case AlgType::SIMPLE:
			renderAlg = [&](CustomColor bgColor, const Ray& ray, float t0, float t1) {
                return simpleComputeVolumeColor(bgColor, *sphere, t0, t1);
			};
			break;
        case AlgType::RAY_MARCH_BACKWARD:
			renderAlg = [&](CustomColor bgColor, const Ray& ray, float t0, float t1) {
                return rayMarchBackward(bgColor, light, *sphere, asymmetryFactor, stepSize, t0, t1, ray);
			};
            break;
        case AlgType::RAY_MARCH_FORWARD:
			renderAlg = [&](CustomColor bgColor, const Ray& ray, float t0, float t1) {
                return rayMarchForward(bgColor, light, *sphere, asymmetryFactor, stepSize, survivalWeight, t0, t1, ray);
			};
            break;
    }

    // 2. Generate single color background image
    Image image = GenImageColor(screenDimension.x, screenDimension.y, scene.getBGColor().getColor());

    // 3. Draw image
    for (int row = 0; row < screenDimension.y; row++) {
		for (int col = 0; col < screenDimension.x; col++) {
            Vector3 rayDir = cam.computeRayDir(row, col, screenDimension);
            Ray ray = { cam.getPosition(), rayDir };

            float t0, t1;

            CustomColor bgColor;
            CustomColor finalColor;

            if (atmosphere.intersect(ray, t0, t1, atmosphere.atmosphereRadius)) {
                bgColor = atmosphere.calcIncidentLight(ray, t1);
            }

            if (atmosphere.intersect(ray, t0, t1, atmosphere.earthRadius) && t1 > 0) {
                float tMax = std::max(0.f, t0);
                bgColor = atmosphere.calcIncidentLight(ray, tMax);
            }

            if (sphere->intersect(ray, t0, t1)) {
                finalColor = renderAlg(bgColor, ray, t0, t1);
            } else {
                finalColor = bgColor;
            }

			ImageDrawPixel(&image, col, row, finalColor.getColor());
		}
    }

    // 4. Load texture
    Texture2D texture = LoadTextureFromImage(image);

    // 5. Release image from RAM since it is already drawn to texture
    UnloadImage(image);

    return texture;
}
