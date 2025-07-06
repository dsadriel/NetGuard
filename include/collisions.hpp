#if !defined(COLLISIONS_HPP)
#define COLLISIONS_HPP

#include <external/glm/vec4.hpp>
#include "matrices.hpp"
#include <cmath>

using namespace glm;
using namespace std;

struct Ray {
	vec4 origin;
	vec4 vector;
	float length;
	bool isDelimited;
	bool inBothDirections;

	Ray() : origin(), vector(), length(0.0f), isDelimited(false), inBothDirections(false) {}
    
	Ray(const vec4 &p, const vec4 &dir, float len = 0.0f, bool delimited = false, bool bothDir = false)
	    : origin(p), vector(dir), length(len), isDelimited(delimited), inBothDirections(bothDir) {}
};

struct Triangle {
    vec4 a;
    vec4 b;
    vec4 c;

    Triangle() : a(), b(), c() {}

    Triangle(const vec4& point1, const vec4& point2, const vec4& point3)
        : a(point1), b(point2), c(point3) {}
};

struct Plane {
    vec4 a;
    vec4 b;
    vec4 c;
    vec4 d;

    Plane() : a(), b(), c(), d() {}

    Plane(const vec4& point1, const vec4& point2, const vec4& point3, const vec4& point4)
        : a(point1), b(point2), c(point3), d(point4) {}
};

struct BoundingBox {
    vec4 boxSize; // Size/dimensions of the box (width, height, depth)
    vec4 origin;  // Center point of the box

    BoundingBox() : boxSize(), origin() {}

    BoundingBox(const vec4& minPoint, const vec4& maxPoint) {
        origin = (minPoint + maxPoint) * 0.5f; // Center point
        boxSize = maxPoint - minPoint;         // Size dimensions
    }

    vec4 getAbsoluteMin() const {
        return origin - boxSize * 0.5f;
    }

    vec4 getAbsoluteMax() const {
        return origin + boxSize * 0.5f;
    }

    vec4 getCenter() const {
        return origin;
    }
};

struct Sphere {
    vec4 center; 
    float radius;

    Sphere() : center(), radius(0.0f) {}

    Sphere(const vec4& c, float r) : center(c), radius(r) {}
};

bool checkCollision(Ray ray, Plane plane);
bool checkCollision(Ray ray, Triangle triangle);
bool checkCollision(BoundingBox box1, BoundingBox box2);
bool checkCollision(Sphere sphere, BoundingBox box);


#endif
