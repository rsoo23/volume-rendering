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
#include "scene_objects/Atmosphere.hpp"

int main()
{
    // screen dimensions { width, height }
    Vector2 screenDimension = { 800, 400 };

    // app
    Application app(screenDimension, "Volume Rendering");

    // scene
    Scene scene(BLACK);

    // atmosphere
    AtmosphereConfig atmosConfig;
    atmosConfig.sunDirection        = { 0, 0, -1 };
	atmosConfig.earthRadius         = 6360e3;
	atmosConfig.atmosphereRadius    = 6420e3;
    atmosConfig.center              = { 0, 0, 0 };
    atmosConfig.rayleighScaleHeight = 7994;
    atmosConfig.mieScaleHeight      = 1200;
	atmosConfig.primaryRaySteps		= 16;
	atmosConfig.sunlightRaySteps	= 8;
	atmosConfig.sunIntensity		= 1000.f;
	atmosConfig.mieAsymmetryFactor	= 0.76f;
    scene.setAtmosphere(std::unique_ptr<Atmosphere>(new Atmosphere(atmosConfig)));

    // light
    // NOTE: will replace light with sunlight
    CustomColor color = { 20, 20, 20, 1};
    Vector3 direction = { 0, 0, 1 };
    scene.addLight(std::unique_ptr<Light>(new Light(color, direction)));

    // sphere
    VolumeMaterial mat;
    mat.color = PURPLE;
    mat.absorptionCoeff = 0.3f;
    mat.scatteringCoeff = 0.3f;
    mat.density = 0.05f;
    Vector3 spherePos = { 0, atmosConfig.earthRadius + 40005, 0 };
    float sphereRadius = 5.f;
    scene.addSceneObject(std::unique_ptr<SceneObject>(new Sphere(mat, spherePos, sphereRadius)));

    // camera
    Vector3 camPos = { 0, atmosConfig.earthRadius + 40000, 0 };
    // NOTE: will add fov and direction
    CustomCamera camera(camPos);

    // ray marching algorithm
    enum AlgType algType = AlgType::RAY_MARCH_FORWARD;

    RayMarchConfig rmConfig;
    rmConfig.stepSize                   = 0.1f;
    rmConfig.asymmetryFactor            = 0.2f;
	rmConfig.survivalWeight             = 10.f;
	rmConfig.russianRouletteThreshold   = 1e-3f;

    FBMConfig fbmConfig;
    fbmConfig.freq				= 0.5f;
	fbmConfig.octaves			= 5;
	fbmConfig.lacunarity		= 2.f;
	fbmConfig.h					= 0.4f;
	fbmConfig.offset			= 0.75f;
	fbmConfig.smoothStepLow		= 0.8f;
	fbmConfig.smoothStepHigh	= 1.0f;

    // renderer
    Renderer renderer;

    // texture
    Timer timer;
    Texture2D textureResult = renderer.render(rmConfig, fbmConfig, screenDimension, scene, camera, algType);
    timer.stop();
    Vector2 texturePos = {
		(screenDimension.x - textureResult.width) * 0.5f,
		(screenDimension.y - textureResult.height) * 0.5f
	};
    CustomColor textureTint = WHITE;

    // main loop
    app.run([&]() {
        app.drawTexture(scene.getBGColor(), textureResult, texturePos, textureTint);
	});

	return 0;
}
