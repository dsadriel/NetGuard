#include "collisions.hpp"

using namespace std;

bool checkCollision(Ray ray, Triangle triangle) {
	// Möller–Trumbore algorithm
	// Adapted from: https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
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
	// To check the collision between a ray and a plane 
	// we can simply check if the ray intersects with two triangles that form the plane.
	
	return checkCollision(ray, Triangle(plane.a, plane.b, plane.c)) ||
	       checkCollision(ray, Triangle(plane.a, plane.c, plane.d));
}

bool checkCollision(BoundingBox box1, BoundingBox box2) {
	// A bounding box collides with another bounding box if all three axes (x, y, z) overlap.
	
	vec4 min1 = box1.getAbsoluteMin();
	vec4 max1 = box1.getAbsoluteMax();
	vec4 min2 = box2.getAbsoluteMin();
	vec4 max2 = box2.getAbsoluteMax();

	if (min1.x > max2.x || max1.x < min2.x)
		return false; // First box is on the left or right the second (no overlap on x-axis)

	if (min1.y > max2.y || max1.y < min2.y)
		return false; // First box is above or below the second (no overlap on y-axis)

	if (min1.z > max2.z || max1.z < min2.z)
		return false; // First box is in front of or behind the second (no overlap on z-axis)

	return true; // Boxes overlap in all three axes
}

bool checkCollision(Sphere sphere1, Sphere sphere2) {
	// A sphere collides with another sphere if the distance between 
	// their centers is less than or equal to the sum of their radius

	vec4 distanceVector = sphere1.center - sphere2.center;
	float distanceSquared = dotproduct(distanceVector, distanceVector);

	float radiusSum = sphere1.radius + sphere2.radius;
	float radiusSumSquared = radiusSum * radiusSum;

	return distanceSquared <= radiusSumSquared;
}

bool checkCollision(Sphere sphere, BoundingBox box) {
	// A sphere collides with a box if the distance from the sphere's center
	// to the closest point on the box is less than or equal to the sphere's radius

	vec4 boxMin = box.getAbsoluteMin();
	vec4 boxMax = box.getAbsoluteMax();

	vec4 closestPoint;
	closestPoint.x = fmax(boxMin.x, fmin(sphere.center.x, boxMax.x));
	closestPoint.y = fmax(boxMin.y, fmin(sphere.center.y, boxMax.y));
	closestPoint.z = fmax(boxMin.z, fmin(sphere.center.z, boxMax.z));

	vec4 distanceVec = sphere.center - closestPoint;
	float distanceSquared = dotproduct(distanceVec, distanceVec);

	return distanceSquared <= (sphere.radius * sphere.radius);
}