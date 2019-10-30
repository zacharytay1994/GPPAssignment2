#pragma once

template <class T>
class Vec2 {
public:
	T x_;
	T y_;
public:
	Vec2(T x, T y) 
		:
		x_(x),
		y_(y)
	{}
	Vec2& operator=(const Vec2& rhs) {
		x_ = rhs.x_;
		y_ = rhs.y_;
		return *this;
	}
	Vec2& operator+=(const Vec2& rhs) {
		x_ += rhs.x_;
		y_ += rhs.y_;
		return *this;
	}
	Vec2 operator+(const Vec2& rhs) {
		return { x_ + rhs.x_, y_ + rhs.y_ };
	}
	Vec2& operator*=(const float& val) {
		x_ *= val;
		y_ *= val;
		return *this;
	}
	Vec2 operator*(const float& val) {
		return { x_ * val, y_ * val };
	}
	Vec2& operator-=(const Vec2& rhs) {
		x_ -= rhs.x_;
		y_ -= rhs.y_;
		return *this;
	}
	Vec2 operator-(const Vec2& rhs) {
		return { x_ - rhs.x_, y_ - rhs.y_ };
	}
	T operator*(const Vec2& rhs) {
		return x_ * rhs.x_ + y_ * rhs.y_;
	}
};
