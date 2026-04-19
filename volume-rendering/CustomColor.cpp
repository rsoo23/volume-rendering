#include "pch.h"
#include "CustomColor.hpp"

CustomColor::CustomColor() : r(0), g(0), b(0), a(1) {}

CustomColor::CustomColor(const Color& color) :
    r(color.r / 255.f),
    g(color.g / 255.f),
    b(color.b / 255.f),
    a(1.f) {}

CustomColor::CustomColor(float r, float g, float b, float a = 1.f) : r(r), g(g), b(b), a(a) {} 

CustomColor::CustomColor(const CustomColor& other) {
    this->r = other.r;
    this->g = other.g;
    this->b = other.b;
    this->a = 1.f;
}

CustomColor::~CustomColor() {}

CustomColor& CustomColor::operator=(const CustomColor& other) {
    if (this != &other) {
		this->r = other.r;
		this->g = other.g;
		this->b = other.b;
        this->a = 1.f;
    }
    return *this;
}

CustomColor CustomColor::operator+(const CustomColor& c2) const {
    CustomColor newColor(
        this->r + c2.r,
		this->g + c2.g,
		this->b + c2.b,
        1.f
	);
    return newColor;
}

CustomColor& CustomColor::operator+=(const CustomColor& c2) {
    this->r += c2.r;
    this->g += c2.g;
    this->b += c2.b;
    return *this;
}

CustomColor CustomColor::operator*(const CustomColor& c2) const {
    CustomColor newColor(
		this->r * c2.r,
		this->g * c2.g,
		this->b * c2.b,
        1.f
    );
    return newColor;
}

CustomColor& CustomColor::operator*=(const CustomColor& c2) {
    this->r *= c2.r;
    this->g *= c2.g;
    this->b *= c2.b;
    return *this;
}

CustomColor& CustomColor::operator*=(float scaleFactor) {
    this->r *= scaleFactor;
    this->g *= scaleFactor;
    this->b *= scaleFactor;
    return *this;
}

CustomColor CustomColor::operator*(float scaleFactor) const {
    CustomColor newColor(
		this->r * scaleFactor,
		this->g * scaleFactor,
		this->b * scaleFactor,
        1.f
    );
    return newColor;
}

Color CustomColor::getColor() const {
    return {
        (unsigned char)(clampColor(this->r) * 255),
        (unsigned char)(clampColor(this->g) * 255),
        (unsigned char)(clampColor(this->b) * 255),
        255
    };
}

std::ostream& operator<<(std::ostream& os, const CustomColor& color) {
	os << "r: " << color.r << ", g: " << color.g << ", b: " << color.b << ", a: " << color.a << "\n";
	return os;
}

float CustomColor::clampColor(float value) const {
    return std::max(0.f, std::min(value, 1.f));
}
