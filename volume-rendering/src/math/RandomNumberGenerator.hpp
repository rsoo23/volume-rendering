#pragma once

class RandomNumberGenerator {
	public:
		RandomNumberGenerator();

		template<typename T>
		T uniform(T lowerBound, T upperBound);

	private:
		std::mt19937 mersenneTwisterEngine;
};

template<typename T>
T RandomNumberGenerator::uniform(T lowerBound, T upperBound) {
	std::uniform_real_distribution<T> dis(lowerBound, upperBound);
	return dis(this->mersenneTwisterEngine);
}
