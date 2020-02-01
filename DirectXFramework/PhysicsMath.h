#pragma once

#include "Vec3.h"

#include <math.h>

class PMat3 {
public:
	Vecf3 x_;
	Vecf3 y_;
	Vecf3 z_;

	PMat3()
		:
		x_({ 1.0f, 0.0f, 0.0f }),
		y_({ 0.0f, 1.0f, 0.0f }),
		z_({ 0.0f, 0.0f, 1.0f })
	{}

	PMat3(const Vecf3& x, const Vecf3& y, const Vecf3& z)
		:
		x_(x),
		y_(y),
		z_(z)
	{}

	Vecf3 operator*(const Vecf3& v) {
		return { x_.x * v.x + y_.x * v.y + z_.z * v.z,
				x_.x * v.x + y_.y * v.y + z_.y * v.z,
				x_.z * v.x + y_.z * v.y + z_.z * v.z };
	};

	PMat3 operator*(const PMat3& rhs) {
		return { *this * rhs.x_, *this * rhs.y_, *this * rhs.z_ };
	}

	PMat3 Transpose() {
		return { {x_.x, y_.x, z_.x},{x_.y, y_.y, z_.y},{x_.z, y_.z, z_.z} };
	}

	PMat3 Abs() {
		using namespace std;
		return { {abs(x_.x), abs(x_.y), abs(x_.z)},
				{abs(y_.x), abs(y_.y), abs(y_.z)},
				{abs(z_.x), abs(z_.y), abs(z_.z)} };
	}

	Vecf3 GetColumn(const int& index) {
		switch (index) {
		case 0:
			return { x_.x, y_.x, z_.x };
			break;
		case 1:
			return { x_.y, y_.y, z_.y };
			break;
		case 2: 
			return { x_.z, y_.z, z_.z };
			break;
		}
		return { 0.0f, 0.0f, 0.0f };
	}
};

struct QuaternionUWU {
public:
	float x;
	float y;
	float z;
	float w;

	QuaternionUWU() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	QuaternionUWU(const float& x_, const float& y_, const float& z_, const float& w_)
		:
		x(x_),
		y(y_),
		z(z_),
		w(w_)
	{}

	QuaternionUWU operator*(const QuaternionUWU& rhs) {
		return { w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
				w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
				w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w,
				w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z };
	}
};

struct Transform {
public:
	Vecf3 position_;
	PMat3 rotation_;

	Vecf3 operator*(const Vecf3& v) {
		return (rotation_ * v) + position_;
	}
};

class PhysicsMath {
public:
	static Vecf3 AbsVec(const Vecf3& v) {
		return { std::abs(v.x), std::abs(v.y), std::abs(v.z) };
	}

	static PMat3 QToMat(const QuaternionUWU& q) {
		float qx2 = q.x + q.x;
		float qy2 = q.y + q.y;
		float qz2 = q.z + q.z;
		float qxqx2 = q.x * qx2;
		float qxqy2 = q.x * qy2;
		float qxqz2 = q.x * qz2;
		float qxqw2 = q.w * qx2;
		float qyqy2 = q.y * qy2;
		float qyqz2 = q.y * qz2;
		float qyqw2 = q.w * qy2;
		float qzqz2 = q.z * qz2;
		float qzqw2 = q.w * qz2;

		PMat3 temp = PMat3();
		temp.x_ = { 1.0f - qyqy2 - qzqz2, qxqy2 + qzqw2, qxqz2 - qyqw2 };
		temp.y_ = { qxqy2 - qzqw2, 1.0f - qxqx2 - qzqz2, qyqz2 + qxqw2 };
		temp.z_ = { qxqz2 + qyqw2, qyqz2 - qxqw2, 1.0f - qxqx2 - qyqy2 };
		return temp;
	}

	static float Invert(const float& f) {
		return f != 0.0f ? 1.0f / f : 0.0f;
	}

	static QuaternionUWU AngularVelToQuart(const Vecf3& av, QuaternionUWU old, const float& dt) {
		QuaternionUWU q = { av.x * dt, av.y * dt, av.z * dt, 0.0f };
		q = q * old;
		old.x += q.x * 0.5f;
		old.y += q.y * 0.5f;
		old.z += q.z * 0.5f;
		old.w += q.w * 0.5f;

		return NormalizeQuart(q);
	}

	static QuaternionUWU NormalizeQuart(const QuaternionUWU& q) {
		float x = q.x;
		float y = q.y;
		float z = q.z;
		float w = q.w;
		float d = q.w * q.w + q.x * q.x + q.y * q.y * q.z * q.z;

		if (d == 0)
		{
			w = 1.0f;
		}

		d = 1.0f / sqrt(d);

		if (d > 0.00000001f)
		{
			x *= d;
			y *= d;
			z *= d;
			w *= d;
		}

		return { x,y,z,w };
	}
};