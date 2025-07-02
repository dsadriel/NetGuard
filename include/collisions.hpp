#if !defined(COLLISIONS_HPP)
#define COLLISIONS_HPP

#include <external/glm/vec4.hpp>

using namespace glm;

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

bool checkCollision(Ray ray, Plane plane);
bool checkCollision(Ray ray, Triangle triangle);

#endif
