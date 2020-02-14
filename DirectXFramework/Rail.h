#pragma once

#include "Block.h"

class Rail : public Block
{
public:
	enum class Direction {
		Vertical,
		Horizontal,
		RTopCurved,
		RBottomCurved,
		LBottomCurved,
		LTopCurved
	};
private:
	Direction direction_ = Direction::Horizontal;

public:
	Rail(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl);
	void Update(const float& dt) override;
	void Render() override;

	Direction GetDirection() { return direction_; }
	void SetDirection(Direction direction);

	QuaternionUWU ToQuaternion(Vecf3 rot) // yaw (Z), pitch (Y), roll (X)
	{
		// Abbreviations for the various angular functions
		double cy = cos(rot.z * 0.5);
		double sy = sin(rot.z * 0.5);
		double cp = cos(rot.y * 0.5);
		double sp = sin(rot.y * 0.5);
		double cr = cos(rot.x * 0.5);
		double sr = sin(rot.x * 0.5);

		QuaternionUWU q;
		q.w = cy * cp * cr + sy * sp * sr;
		q.x = cy * cp * sr - sy * sp * cr;
		q.y = sy * cp * sr + cy * sp * cr;
		q.z = sy * cp * cr - cy * sp * sr;

		return q;
	}
};

