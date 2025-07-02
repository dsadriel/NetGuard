#include "collisions.hpp"
#include "matrices.hpp"

// Möller–Trumbore algorithm
// Adaptado de: https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
bool checkCollision(Ray ray, Triangle triangle) {
	const float epsilon = 1e-6f;

	vec4 edge1 = triangle.b - triangle.a;
	vec4 edge2 = triangle.c - triangle.a;

	vec4 ray_cross_e2 = crossproduct(ray.vector, edge2);
	float det = dot(edge1, ray_cross_e2);

    if (abs(det) < epsilon)
		return false; // This ray is parallel to this triangle.

	float inv_det = 1.0 / det;
	vec4 s = ray.origin - triangle.a;
	float u = inv_det * dot(s, ray_cross_e2);

	if ((u < 0 && abs(u) > epsilon) || (u > 1 && abs(u - 1) > epsilon))
		return false; // This means that the intersection point is outside the triangle.

	vec4 s_cross_e1 = crossproduct(s, edge1);
	float v = inv_det * dotproduct(ray.vector, s_cross_e1);

	if ((v < 0 && abs(v) > epsilon) || (u + v > 1 && abs(u + v - 1) > epsilon))
		return false; // This means that the intersection point is outside the triangle.

	float t = inv_det * dot(edge2, s_cross_e1);
	if (t > epsilon && (ray.isDelimited == false || (ray.isDelimited && t < ray.length))) {
        return true; // This means that the ray intersects the triangle within the specified length.
	} else 
		return false;
}

bool checkCollision(Ray ray, Plane plane) {
	return checkCollision(ray, Triangle(plane.a, plane.b, plane.c)) ||
	       checkCollision(ray, Triangle(plane.a, plane.c, plane.d));
}
