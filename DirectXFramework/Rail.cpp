#include "Rail.h"

Rail::Rail(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl):
	Block::Block(image, gfx, input, rl)
{
	SetQuatRotate(1);
	SetRotation(ToQuaternion(Vecf3(0.f, PI/2, 0.f)));
}

void Rail::Update(const float& dt)
{
	Block::Update(dt);
}

void Rail::Render()
{
	Block::Render();
	cube_.HandleDraw();
}

void Rail::SetDirection(Direction direction) {

	// Set direction
	direction_ = direction;

	// Update image
	if (direction_ == Direction::Vertical) {
		
		cube_.SetTextureKey("rail");
		SetRotation(ToQuaternion(Vecf3(0.f, 0.f, 0.f)));

	} else if (direction_ == Direction::RTopCurved) {

		cube_.SetTextureKey("railcurved");
		SetRotation(ToQuaternion(Vecf3(0.f, 0.f, 0.f)));

	} else if (direction_ == Direction::RBottomCurved) {

		cube_.SetTextureKey("railcurved");
		SetRotation(ToQuaternion(Vecf3(0.f, PI/2, 0.f)));

	} else if (direction_ == Direction::LBottomCurved) {

		cube_.SetTextureKey("railcurved");
		SetRotation(ToQuaternion(Vecf3(0.f, PI, 0.f)));

	}
	else if (direction_ == Direction::LTopCurved) {

		cube_.SetTextureKey("railcurved");
		SetRotation(ToQuaternion(Vecf3(0.f, 3*PI/2, 0.f)));

	} else {

		cube_.SetTextureKey("rail");
		SetRotation(ToQuaternion(Vecf3(0.f, PI/2, 0.f)));

	}
}