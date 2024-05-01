#include "MyMtVector3.h"
#include <cmath>

Vector3 MyMtVector3::Add(const Vector3& v1, const Vector3& v2) {
	Vector3 v3;
	v3.x = v1.x + v2.x;
	v3.y = v1.y + v2.y;
	v3.z = v1.z + v2.z;
	return v3;
}

Vector3 MyMtVector3::Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 v3;
	v3.x = v1.x - v2.x;
	v3.y = v1.y - v2.y;
	v3.z = v1.z - v2.z;
	return v3;
}

Vector3 MyMtVector3::Multiply(float scalar, const Vector3& v) {
	Vector3 v2;
	v2.x = v.x * scalar;
	v2.y = v.y * scalar;
	v2.z = v.z * scalar;
	return v2;
}
float MyMtVector3::Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float MyMtVector3::Length(const Vector3& v) {
	return sqrtf(Dot(v, v));
}

Vector3 MyMtVector3::Normalize(const Vector3& v) {
	Vector3 v2;
	v2.x = v.x / Length(v);
	v2.y = v.y / Length(v);
	v2.z = v.z / Length(v);
	return v2;
}

Vector3 MyMtVector3::Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 v3;
	v3 = Add(v1, Multiply(t, Subtract(v2,v1)));
	return v3;
}

Vector3 MyMtVector3::Slerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 v3;
	Vector3 nV1 = Normalize(v1);
	Vector3 nV2 = Normalize(v2);
	float dot = Dot(nV1, nV2);
	//
	if (dot > 1.0f) {
		dot = 1.0f;
	}
	float theta = std::acos(dot);
	float sinTheta = std::sin(theta);
	float sinThetaFrom = std::sin((1 - t) * theta);
	float sinThetaTo = std::sin(t * theta);
	//
	Vector3 nCompVector;
	if (sinTheta < 1.0e-5) {
		nCompVector = nV1;
	} else {
		nCompVector = Add(Multiply(sinThetaFrom / sinTheta, nV1), Multiply(sinThetaTo / sinTheta, nV2));
	}
	float length1 = Length(v1);
	float length2 = Length(v2);
	float length = Lerp({length1}, {length2}, t).x;

	v3 = Multiply(length, nCompVector);
	return v3;
}
