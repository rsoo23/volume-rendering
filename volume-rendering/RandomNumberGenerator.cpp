#include "pch.h"
#include "RandomNumberGenerator.hpp"

RandomNumberGenerator::RandomNumberGenerator() : mersenneTwisterEngine(std::random_device{}()) {}
