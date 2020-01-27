#pragma once

#include "MathStuff.h"

template <typename T>
class Vec2
{
public:
	Vec2() = default;
	Vec2(T x, T y)
		:
		x(x),
		y(y)
	{}
	Vec2(const Vec2& vect)
		:
		Vec2(vect.x, vect.y)
	{}
	template <typename T2>
	explicit operator Vec2<T2>() const
	{
		return{ (T2)x,(T2)y };
	}
	T		LenSq() const
	{
		return sq(*this);
	}
	T		Len() const
	{
		return sqrt(LenSq());
	}
	Vec2& Normalize()
	{
		const T length = Len();
		x /= length;
		y /= length;
		return *this;
	}
	Vec2	GetNormalized() const
	{
		Vec2 norm = *this;
		norm.Normalize();
		return norm;
	}
	Vec2	operator-() const
	{
		return Vec2(-x, -y);
	}
	Vec2& operator=(const Vec2& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	Vec2& operator+=(const Vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vec2& operator-=(const Vec2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	Vec2	operator+(const Vec2& rhs) const
	{
		return Vec2(*this) += rhs;
	}
	Vec2	operator-(const Vec2& rhs) const
	{
		return Vec2(*this) -= rhs;
	}
	Vec2& operator*=(const T& rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	Vec2	operator*(const T& rhs) const
	{
		return Vec2(*this) *= rhs;
	}
	Vec2& operator/=(const T& rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
	Vec2	operator/(const T& rhs) const
	{
		return Vec2(*this) /= rhs;
	}
	bool	operator==(const Vec2& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool	operator!=(const Vec2& rhs) const
	{
		return !(*this == rhs);
	}
public:
	T x;
	T y;
};

typedef Vec2<float> Vecf2;
typedef Vec2<double> Vecd2;
typedef Vec2<int> Veci2;