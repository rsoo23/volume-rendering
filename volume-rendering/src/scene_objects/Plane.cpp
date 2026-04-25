#include "pch.h"
#include "Plane.hpp"

Plane::Plane() :
	color(WHITE),
	normal(Vector3Normalize({ 0, 1, 0 })),
	d(0.f),
	p0(Vector3Scale(normal, d)) {}

Plane::Plane(CustomColor color, Vector3 normal, float d) :
	color(color),
	normal(Vector3Normalize(normal)),
	d(d),
	p0(Vector3Scale(normal, d)) {}

// all vectors are assumed to be normalized
// 1. A · B = 0
// 2. (p - p0) · n = 0
// - p: point of intersect between ray and plane
// - p0: point on plane (derived from normal and d)
// - n: normal of plane
// 3. Ray equation: p = rayOrigin + t * rayDirection
// 4. Substitute p into equation in 2 and rearrange to find t:
// - t = (p0 - rayOrigin) · n  / (rayDirection · n)
bool Plane::intersect(const Ray& ray, float& t) const {
	float denom = Vector3DotProduct(this->normal, ray.direction);

	if (denom > 1e-6) {
		Vector3 rayOriginToPointOnPlane = Vector3Subtract(this->p0, ray.position);
		t = Vector3DotProduct(rayOriginToPointOnPlane, this->normal) / denom;
		return (t >= 0);
	}
	return false;
}

CustomColor Plane::getColor() const {
	return this->color;
}

Vector3 Plane::getNormal() const {
	return this->normal;
}
