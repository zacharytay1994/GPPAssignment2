#pragma once

#include "Vec2.h"
#include "MathStuff.h"
#include <algorithm>
#include <cmath>

template <typename T>
class Vec3 : public Vec2<T> {
public:
	Vec3() = default;
	Vec3(T x, T y, T z)
		:
		Vec2<T>(x, y),
		z(z)
	{}
	Vec3(const Vec3& vec3)
		:
		Vec3(vec3.x, vec3.y, vec3.z)
	{}
	template <typename T2>
	explicit operator Vec3<T2>() const
	{
		return{ (T2)this->x, (T2)this->y, (T2)z };
	}
	T		LenSq() const
	{
		return sq(*this);
	}
	T		Len() const
	{
		return sqrt(LenSq());
	}
	Vec3& Normalize()
	{
		const T length = Len();
		this->x /= length;
		this->y /= length;
		z /= length;
		return *this;
	}
	Vec3	GetNormalized() const
	{
		Vec3 norm = *this;
		norm.Normalize();
		return norm;
	}
	Vec3	Max(const float max) {
		this->x = this->x < max ? this->x : max;
		this->y = this->y < max ? this->y : max;
		this->z = this->z < max ? this->z : max;
		return *this;
	}
	Vec3	Min(const float min) {
		this->x = this->x > min ? this->x : min;
		this->y = this->y > min ? this->y : min;
		this->z = this->z > min ? this->z : min;
		return *this;
	}

	Vec3	operator-() const
	{
		return Vec3(-this->x, -this->y, -z);
	}
	Vec3& operator=(const Vec3& rhs)
	{
		this->x = rhs.x;
		this->y = rhs.y;
		z = rhs.z;
		return *this;
	}
	Vec3& operator+=(const Vec3& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		z += rhs.z;
		return *this;
	}	
	Vec3& operator-=(const Vec3& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	Vec3	operator+(const Vec3& rhs) const
	{
		return Vec3(*this) += rhs;
	}
	Vec3	operator-(const Vec3& rhs) const
	{
		return Vec3(*this) -= rhs;
	}
	Vec3& operator*=(const T& rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		z *= rhs;
		return *this;
	}
	Vec3	operator*(const T& rhs) const
	{
		return Vec3(*this) *= rhs;
	}
	float operator*(const Vec3& rhs) const {
		float result = this->x * rhs.x + this->y * rhs.y + z * rhs.z;
		return result;
	}
	Vec3 operator%(const Vec3& rhs) const {
		return { this->y * rhs.z - z * rhs.y, z * rhs.x - this->x * rhs.z, this->x * rhs.y - this->y * rhs.x };
	}
	Vec3& operator/=(const T& rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
		z /= rhs;
		return *this;
	}
	Vec3	operator/(const T& rhs) const
	{
		return Vec3(*this) /= rhs;
	}
	bool	operator==(const Vec3& rhs) const
	{
		return this->x == rhs.x && this->y == rhs.y && z == rhs.z;
	}
	bool	operator!=(const Vec3& rhs) const
	{
		return !(*this == rhs);
	}
public:
	T z;
};

typedef Vec3<float> Vecf3;
typedef Vec3<double> Vecd3;
typedef Vec3<int> Veci3;

static Vecf3 RotateVectorY(Vecf3 vector, float angle)
{
	return Vecf3((vector.x * std::cos(angle) + vector.z * -std::sin(angle)), 0.0f, (vector.x * std::sin(angle) + vector.z * std::cos(angle)));
}

static float AngleBetweenY(Vecf3 a, Vecf3 b)
{
	return std::atan2(a.z - b.z, a.x - b.x) * 57.2957795f + 90;
}