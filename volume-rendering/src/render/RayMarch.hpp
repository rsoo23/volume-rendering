#pragma once

#include "utils/CustomColor.hpp"
#include "math/RandomNumberGenerator.hpp"
#include "scene_objects/Light.hpp"
#include "scene_objects/Sphere.hpp"
#include "scene_objects/IVolumeObject.hpp"

struct RayMarchConfig {
	float asymmetryFactor = 0.2f;
	float stepSize = 0.1f;
	float survivalWeight = 10.f; // only used by forward
	float russianRouletteThreshold = 1e-3f; // only used by forward
};

// only used in forward
// - octaves: number of layers
// - lacunarity: gap between successive frequencies
// - h: fractal increment parameter
struct FBMConfig {
    float freq				= 0.5f;
	size_t octaves			= 5;
	float lacunarity		= 2.f;
	float h					= 0.4f;
	float offset			= 0.75f;
	float smoothStepLow		= 0.8f;
	float smoothStepHigh	= 1.0f;
};

namespace RayMarch {
	CustomColor backward(
		const RayMarchConfig& config,
		const Ray& ray, float t0, float t1,
		const Light& light,
		const IVolumeObject& volume,
		CustomColor bgColor
	);

	CustomColor forward(
		const RayMarchConfig& rmConfig,
		const FBMConfig& fbmConfig,
		const Ray& ray, float t0, float t1,
		const Light& light,
		const IVolumeObject& volume,
		RandomNumberGenerator& rng,
		CustomColor bgColor
	);
}
