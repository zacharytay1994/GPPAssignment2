#pragma once

template <class T>
class Vec2 {
private:
	T x_;
	T y_;
public:
	Vec2(T x, T y) 
		:
		x_(x),
		y_(y)
	{}
	Vec2& operator+=(const Vec2& rhs) {
		x_ += rhs.x_;
		y_ += rhs.y_;
		return *this;
	}
	Vec2 operator+(const Vec2& rhs) const {
		return *this += rhs;
	}
};
