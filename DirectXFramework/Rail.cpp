#include "Rail.h"

Rail::Rail(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, std::shared_ptr<ResourceLibrary> rl):
	Entity::Entity(image, gfx, input, rl)
{
	SetQuatRotate(1);
	SetRotation(ToQuaternion(Vecf3(0.f, PI/2, 0.f)));
}

void Rail::Update(const float& dt)
{
	Entity::Update(dt);
}

void Rail::Render()
{
	Entity::Render();
	//cube_.HandleDraw();
}

void Rail::SetDirection(Direction direction) {

	// Set direction
	direction_ = direction;

	// Update image
	if (direction_ == Direction::Vertical) {
		
		cube_.SetTextureKey(is_ghost_ ? "grail" : "rail");
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

		cube_.SetTextureKey(is_ghost_?"grail":"rail");
		SetRotation(ToQuaternion(Vecf3(0.f, PI/2, 0.f)));

	}

}

void Rail::MakeGhost()
{
	is_ghost_ = true;
}
