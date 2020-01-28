#pragma once

class Color {
private:
	// 4 8-bit values (32 bit) representing color channels,
	// i.e. alpha: 1, red: 128, green: 64, blue: 32
	// 00000001 | 10000000 | 01000000 | 00100000
	// bitmask OR
	// 00000001100000000100000000100000
	unsigned int color;
public:
	constexpr Color() : color() {}
	constexpr Color(const Color& col) : color(col.color) {}
	constexpr Color(unsigned int color) : color(color) {}
	constexpr Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
		:
		color((a << 24u) | (r << 16u) | (g << 8u) | b)
	{}
	constexpr Color(unsigned char r, unsigned char g, unsigned char b)
		:
		color((r << 16u) | (g << 8u) | b)
	{}
	Color& operator=(Color rhs) {
		color = rhs.color;
		return *this;
	}
	bool operator==(const Color& rhs) const {
		return color == rhs.color;
	}
	bool operator!=(const Color& rhs) const {
		return !(*this == rhs);
	}
	constexpr unsigned char GetA() const {
		return color >> 24u;
	}
	constexpr unsigned char GetR() const {
		return (color >> 16u) & 0xFFu;
	}
	constexpr unsigned char GetG() const {
		return(color >> 8u) & 0xFFu;
	}
	constexpr unsigned char GetB() const {
		return color & 0xFFu;
	}
	void SetA(unsigned char a) {
		color = (color & 0xFFFFFFu) | (a << 24u);
	}
	void SetR(unsigned char r) {
		color = (color & 0xFF00FFFFu) | (r << 16u);
	}
	void SetG(unsigned char g) {
		color = (color & 0xFFFF00FFu) | (g << 8u);
	}
	void SetB(unsigned char b) {
		color = (color & 0xFFFFFF00u) | b;
	}
};

namespace Colors {
	static constexpr Color MakeRGB(unsigned char r, unsigned char g, unsigned char b) {
		return (r << 16) | (g << 8) | b;
	}
	static constexpr Color White = MakeRGB(255u, 255u, 255u);
	static constexpr Color Black = MakeRGB(0u, 0u, 0u);
	static constexpr Color Red = MakeRGB(255u, 0u, 0u);
	static constexpr Color Green = MakeRGB(0u, 255u, 0u);
	static constexpr Color Blue = MakeRGB(0u, 0u, 255u);
}