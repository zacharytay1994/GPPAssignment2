#include "Button.h"

Button::Button(const std::string& image, std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, 
	std::shared_ptr<ResourceLibrary> rl, std::shared_ptr<Player> player, Scene* scene, const Vecf3& btnSize, std::string id ) :
	Entity(image, gfx, input, rl),
	cube_before_(gfx, input, "progress", rl),
	cube_after_(gfx, input, "progress", rl)
{
	player_ = player;
	id_ = id;
	scene_ = scene;

	cube_.SetScaleX(btnSize.x);
	cube_.SetScaleY(btnSize.y);
	cube_.SetScaleZ(btnSize.z);

	cube_after_.SetScaleX(btnSize.y/2);
	cube_after_.SetScaleY(btnSize.y/2);
	cube_before_.SetScaleX(btnSize.y/2);
	cube_before_.SetScaleY(btnSize.y/2);

	YOffset = btnSize.y;
	
}

void Button::Update(const float& dt)
{
	Entity::Update(dt);


	const float btnLeft = position_.x - cube_.GetScaleX();
	const float btnRight = position_.x + cube_.GetScaleX();
	const float btnTop = position_.z + cube_.GetScaleZ();
	const float btnBottom = position_.z - cube_.GetScaleZ();
	const Vecf3 playerPos = player_->GetPosition();
	playerOnTop_ = playerPos.x > btnLeft && playerPos.x < btnRight && playerPos.z > btnBottom && playerPos.z < btnTop;
	progress_ = playerOnTop_ ? progress_ + dt : 0;
	progress_ = progress_ > coolDown_ ? coolDown_ : progress_;
	//activated_ = ;

	if (progress_ == coolDown_) {
		scene_->HandleActiveButton(id_);
	}

	cube_.SetX(position_.x);
	cube_.SetY(position_.y - (playerOnTop_? YOffset :0.0f));
	cube_.SetZ(position_.z);

	if (playerOnTop_) {
		const float yPosition = position_.y - cube_.GetScaleY() / 2;
		cube_before_.SetY(yPosition);
		cube_after_.SetY(yPosition);

		const float xOffSet = cube_.GetScaleX() + cube_after_.GetScaleX() * 2.5;
		cube_before_.SetX(position_.x - xOffSet);
		cube_after_.SetX(position_.x + xOffSet);

		const float zOffSet = cube_.GetScaleZ() - cube_.GetScaleZ() * progress_ / coolDown_;
		cube_before_.SetZ(position_.z + zOffSet);
		cube_after_.SetZ(position_.z - zOffSet);

		const float zScale = progress_ / coolDown_ * cube_.GetScaleZ();
		cube_after_.SetScaleZ(zScale);
		cube_before_.SetScaleZ(zScale);
	}


}

void Button::Render()
{
	Entity::Render();
	cube_.HandleDraw();
	if (playerOnTop_) {
		cube_before_.HandleDraw();
		cube_after_.HandleDraw();
	}
	
}
