#pragma once
#include "Vector3.h"
#include "vector"

class MyMtVector3{
public:
	static Vector3 Add(const Vector3& v1, const Vector3& v2);
	static Vector3 Subtract(const Vector3& v1, const Vector3& v2); 
	static Vector3 Multiply(float scalar, const Vector3& v); 
	static float Dot(const Vector3& v1, const Vector3& v2);
	static float Length(const Vector3& v);
	static Vector3 Normalize(const Vector3& v);
	static Vector3 Min(const Vector3& v, const Vector3& min);
	static Vector3 Max(const Vector3& v, const Vector3& max);
	static Vector3 Clamp(const Vector3& v, const Vector3& min, const Vector3& max);
	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
	static Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);
	static Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);
	static Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);
};

