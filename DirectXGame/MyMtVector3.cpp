#include "MyMtVector3.h"
#include <cmath>
#include <cassert>

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
Vector3 MyMtVector3::Min(const Vector3& v, const Vector3& min) {
	Vector3 v2 = v;
	v2.x = v2.x < min.x ? min.x : v2.x;
	v2.y = v2.y < min.y ? min.y : v2.y;
	v2.z = v2.z < min.z ? min.z : v2.z;
	return v2;
}

Vector3 MyMtVector3::Max(const Vector3& v, const Vector3& max) {
	Vector3 v2 = v;
	v2.x = v2.x > max.x ? max.x : v2.x;
	v2.y = v2.y > max.y ? max.y : v2.y;
	v2.z = v2.z > max.z ? max.z : v2.z;
	return v2;
}
Vector3 MyMtVector3::Clamp(const Vector3& v, const Vector3& min, const Vector3& max) {
	Vector3 v2 = v;
	v2 = Min(v2, min);
	v2 = Max(v2, max);
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

Vector3 MyMtVector3::CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
	const float s = 0.5f; // 数式に出てくる 1/2 のこと。

	float t2 = t * t;  // t の2乗
	float t3 = t2 * t; // t の3乗

	Vector3 e3 = Add(Subtract(Add(Multiply(-1.f, p0), Multiply(3.0f, p1)), Multiply(3.0f, p2)), p3);

	Vector3 e2 = Subtract(Add(Subtract(Multiply(2.0f, p0), Multiply(5.0f, p1)), Multiply(4.0f, p2)), p3);

	Vector3 e1 = Add(Multiply(-1.0f, p0), p2);

	Vector3 e0 = Multiply(2.0f, p1);

	Vector3 ans = Add(Add(Add(Multiply(t3, e3), Multiply(t2, e2)), Multiply(t, e1)), e0);

	return Multiply(s, ans);
}

/// CatmullRomスプライン曲線上の座標を得る
Vector3 MyMtVector3::CatmullRomPosition(const std::vector<Vector3>& points, float t) {
	assert(points.size() >= 4 && "制御点は4点以上必要です");

	// 区間数は制御点の数-1
	size_t division = points.size() - 1;
	// 1区間の長さ(全体を1.0とした割合)
	float areaWidth = 1.0f / division;

	// 区間内の始点を0.0f、終点を1.0fとしたときの現在位置
	float t_2 = std::fmod(t, areaWidth) * division;
	// 下限(0.0f)と上限(1.0f)の範囲に収める
	t_2 = Clamp({t_2,0,0}, {0.0f,0,0}, {1.0f,0,0}).x;

	// 区間番号
	size_t index = static_cast<size_t>(t / areaWidth);
	// 区間番号が上限を超えないように収める
	index =  index + 1 >= points.size() ? points.size() - 2 : index;

	// 4点分のインデックス
	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;

	// 最初の区間のp0はp1を重複使用する
	if (index == 0) {
		index0 = index1;
	}

	// 最後の区間のp3はp2を重複使用する
	if (index3 >= points.size()) {
		index3 = index2;
	}

	// 4点の座標
	const Vector3& p0 = points.at(index0);
	const Vector3& p1 = points.at(index1);
	const Vector3& p2 = points.at(index2);
	const Vector3& p3 = points.at(index3);

	// 4点を指定してCatmul-Rom補間
	return CatmullRomInterpolation(p0, p1, p2, p3, t_2);
}

