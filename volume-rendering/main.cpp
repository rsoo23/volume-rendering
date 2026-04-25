#include "pch.h"
#include "utils/CustomColor.hpp"
#include "scene_objects/Sphere.hpp"
#include "scene_objects/Light.hpp"
#include "scene_objects/CustomCamera.hpp"
#include "render/RayMarch.hpp"
#include "math/RandomNumberGenerator.hpp"
#include "utils/Timer.hpp"
#include "core/Application.hpp"
#include "scene/Scene.hpp"
#include "render/Renderer.hpp"
#include "shared/Types.h"
#include "scene_objects/Plane.hpp"

int main()
{
    // screen dimensions { width, height }
    Vector2 screenDimension = { 800, 400 };

    // app
    Application app(screenDimension, "Volume Rendering");

    // scene
    Scene scene(BLACK);
    scene.addLight(std::unique_ptr<Light>(new Light({ 20, 20, 20, 1 }, { 0, 0, 1 })));
    scene.addSceneObject(std::unique_ptr<SceneObject>(new Sphere(PURPLE, 0.3f, 0.3f, 0.05f, { 0, 0, -20 }, 3.f)));
    scene.addSceneObject(std::unique_ptr<SceneObject>(new Plane(PURPLE, { 0, -1, 0 }, 0)));

    // camera
    CustomCamera camera({ 0, 0, 0 });

    // algorithm
    enum AlgType algType = AlgType::RAY_MARCH_FORWARD;
    const float stepSize = 0.5f;

    // renderer
    Renderer renderer;

    Timer timer;
    Texture2D textureResult = renderer.render(screenDimension, scene, camera, algType);
    timer.stop();

    Vector2 texturePos = {
		(screenDimension.x - textureResult.width) * 0.5f,
		(screenDimension.y - textureResult.height) * 0.5f
	};

    CustomColor textureTint = WHITE;

    app.run([&]() {
        app.drawTexture(scene.getBGColor(), textureResult, texturePos, textureTint);
	});

	return 0;
}
