#include "pch.h"
#include "SimpleCalcVolumeColor.hpp"
#include "scene_objects/Sphere.hpp"
#include "scene_objects/IVolumeObject.hpp"

CustomColor simpleCalcVolumeColor(CustomColor bgColor, const IVolumeObject& volume, float t0, float t1) {
	const float distance = t1 - t0;
	VolumeMaterial material = volume.getMaterial();
    const float transmittance = exp(-distance * material.absorptionCoeff);
	CustomColor newColor = bgColor * transmittance + material.color * (1 - transmittance);

    return newColor;
}
