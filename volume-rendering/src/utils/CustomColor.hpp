#pragma once

#include "raymath.h"

class CustomColor {
	public:
		CustomColor();
		CustomColor(const Color& color);
		CustomColor(float r, float g, float b, float a);
		CustomColor(const CustomColor& other);
		~CustomColor();
		CustomColor& operator=(const CustomColor& other);
		CustomColor operator+(const CustomColor& c2) const;
		CustomColor& operator+=(const CustomColor& c2);
		CustomColor operator*(const CustomColor& c2) const;
		CustomColor& operator*=(const CustomColor& c2);
		CustomColor operator/(const CustomColor& c2) const;
		CustomColor& operator*=(float scaleFactor);
		CustomColor operator*(float scaleFactor) const;
		Color getColor() const;
		friend std::ostream& operator<<(std::ostream& os, const CustomColor& color);

	private:
		float clampColor(float value) const;
		float r, g, b, a;
};