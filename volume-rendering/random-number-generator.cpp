#include "pch.h"
#include "random-number-generator.hpp"

RandomNumberGenerator::RandomNumberGenerator() : mersenneTwisterEngine(std::random_device{}()) {}
