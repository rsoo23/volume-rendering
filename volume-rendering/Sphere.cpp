#include "pch.h"
#include "Sphere.hpp"
#include "Utils.hpp"

Sphere::Sphere() :
	absorptionCoeff(0.1f),
	scatteringCoeff(0.1f),
	density(1.f),
	color(PURPLE),
	center({ 0, 0, -10 }),
	radius(5.f) {}

Sphere::Sphere(float absorptionCoeff, float scatteringCoeff, float density, CustomColor color, Vector3 center, float radius) :
	absorptionCoeff(absorptionCoeff),
	scatteringCoeff(scatteringCoeff),
	density(density),
	color(color),
	center(center),
	radius(radius) {}

// 1. Sphere equation: |p - c|^2 = r^2 where;
// - p is the intersection point on sphere
// - c is center of sphere
// - r is radius of sphere
// 2. Since |v|^2 = v · v we can expand into (p - c) · (p - c) = r^2 
// 3. Equation of vector / ray that intersects the sphere is p = A + tB where
// - A is the ray origin
// - t is the scalar parameter representing how far along we traveled
// - B is the ray direction
// 4. Substitute p into the equation in 2. and expand to get (B · B)t^2 + 2(B · OC)t + (OC · OC - r^2) = 0 where
// - OC is A - c (origin of ray to center of sphere)
// 5. Solve the quadratic equation using modified equation for improved stability, preventing loss of significance
bool Sphere::intersect(const Ray& ray, float& t0, float& t1) const {
	Vector3 oc = Vector3Subtract(ray.position, this->center);

	float a = Vector3DotProduct(ray.direction, ray.direction);
	float b = 2 * Vector3DotProduct(ray.direction, oc);
	float c = Vector3DotProduct(oc, oc) - this->radius * this->radius;

	float discriminant = b * b - 4 * a * c;

	if (discriminant < 0) return false;
	else if (discriminant == 0) t0 = t1 = -0.5 * b / a; 
	else {
		float q = (b > 0) ?
			-0.5 * (b + sqrt(discriminant)) :
            -0.5 * (b - sqrt(discriminant));
        t0 = q / a;
        t1 = c / q;
    }

	// guarantees nearest point is always t0
    if (t0 > t1) std::swap(t0, t1);
    
    return true;
}

CustomColor Sphere::computeVolumeColor(const CustomColor& bgColor, float t0, float t1) const {
	const float distance = t1 - t0;
	return calculateBGColorThroughVolume(bgColor, this->color, this->absorptionCoeff, distance);
}
