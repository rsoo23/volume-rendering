#pragma once

#include "utils/CustomColor.hpp"

enum class AlgType {
    SIMPLE,
    RAY_MARCH_BACKWARD,
    RAY_MARCH_FORWARD
};

struct VolumeMaterial {
	CustomColor color;
	float absorptionCoeff;
	float scatteringCoeff;
	float density;
};
