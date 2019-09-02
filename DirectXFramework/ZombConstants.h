#pragma once

#include <DirectXMath.h>
#include <cmath>

/* Vector 2D Class*/
class Vector2D {
public:
	float x_;
	float y_;
public:
	Vector2D(const float& x, const float& y) : x_(x), y_(y) {}
	Vector2D& operator=(const Vector2D& rhs) {
		x_ = rhs.x_;
		y_ = rhs.y_;
		return *this;
	}
	Vector2D& operator+(const Vector2D& rhs) {
		x_ += rhs.x_;
		y_ += rhs.y_;
		return *this;
	}
	Vector2D& operator-(const Vector2D& rhs) {
		x_ -= rhs.x_;
		y_ -= rhs.y_;
		return *this;
	}
	Vector2D& operator*(const float& scalar) {
		x_ *= scalar;
		y_ *= scalar;
		return *this;
	}
	float operator*(const Vector2D& rhs) {
		return (x_ * rhs.x_) + (y_ * rhs.y_);
	}
	Vector2D Normalized() {
		float magnitude = std::pow((std::pow(x_,2) + std::pow(y_,2)),0.5f);
		if (magnitude != 0.0f) {
			x_ /= magnitude;
			y_ /= magnitude;
		}
		else {
			x_ = 0.0f;
			y_ = 0.0f;
		}
		return *this;
	}
};