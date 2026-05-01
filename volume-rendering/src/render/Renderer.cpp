#include "pch.h"
#include "Renderer.hpp"
#include "math/RandomNumberGenerator.hpp"
#include "render/RayMarch.hpp"
#include "scene_objects/Sphere.hpp"
#include "render/SimpleCalcVolumeColor.hpp"
#include "scene_objects/Plane.hpp"
#include "scene_objects/Atmosphere.hpp"
#include "scene_objects/IVolumeObject.hpp"
#include <cassert>

Renderer::Renderer() {}

Texture2D Renderer::render(
	const RayMarchConfig& rmConfig,
	const FBMConfig& fbmConfig,
    const Vector2 screenDimension,
    const Scene& scene,
    const CustomCamera& cam,
    const AlgType algType
) {
	const Atmosphere* atmosphere = scene.getAtmosphere();
    assert(atmosphere && "Renderer::render - scene has no atmosphere");

    // NOTE: will add multi object handling
    const IVolumeObject* volume = scene.findFirst<IVolumeObject>();
    assert(volume && "Renderer::render - scene has no volume object");

    // NOTE: will add multi light function in the future
	const Light* light = scene.getLights().empty() ? nullptr : scene.getLights()[0].get();
    assert(light && "Renderer::render - scene has no lights");

    RandomNumberGenerator rng;
    const AtmosphereConfig& atmosConfig = atmosphere->getConfig();

    // 1. Select algorithm
    std::function<CustomColor(CustomColor bgColor, const Ray& ray, float t0, float t1)> renderAlg;
    switch (algType) {
		case AlgType::SIMPLE:
			renderAlg = [&](CustomColor bgColor, const Ray& ray, float t0, float t1) {
                return simpleCalcVolumeColor(bgColor, *volume, t0, t1);
			};
			break;
        case AlgType::RAY_MARCH_BACKWARD:
			renderAlg = [&](CustomColor bgColor, const Ray& ray, float t0, float t1) {
                return RayMarch::backward(rmConfig, ray, t0, t1, *light, *volume, bgColor);
			};
            break;
        case AlgType::RAY_MARCH_FORWARD:
			renderAlg = [&](CustomColor bgColor, const Ray& ray, float t0, float t1) {
                return RayMarch::forward(rmConfig, fbmConfig, ray, t0, t1, *light, *volume, rng, bgColor);
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

            if (atmosphere->intersect(ray, t0, t1, atmosConfig.atmosphereRadius)) {
                bgColor = atmosphere->calcIncidentLight(ray, t1);
            }

            if (atmosphere->intersect(ray, t0, t1, atmosConfig.earthRadius) && t1 > 0) {
                float tMax = std::max(0.f, t0);
                bgColor = atmosphere->calcIncidentLight(ray, tMax);
            }

            if (volume->intersect(ray, t0, t1)) {
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
