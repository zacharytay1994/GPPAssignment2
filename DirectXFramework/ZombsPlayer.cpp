#include "ZombsPlayer.h"

ZombsPlayer::ZombsPlayer(std::shared_ptr<Graphics>& graphics, const std::wstring & spriteresource, const int & width, const int & height)
	:
	ZombsEntity(graphics,  spriteresource, width, height)
{
	SetMaxSpeed(4.0f);
	SetAcceleration(0.5f);
}

ZombsPlayer::~ZombsPlayer()
{
}

void ZombsPlayer::Update(std::shared_ptr<Input>& input, const float& dt)
{
	float up = 0.0f;
	float down = 0.0f;
	float left = 0.0f;
	float right = 0.0f;
	if (input->KeyIsDown('W')) {
		up = -1.0f;
	}
	if (input->KeyIsDown('S')) {
		down = 1.0f;
	}
	if (input->KeyIsDown('A')) {
		left = -1.0f;
	}
	if (input->KeyIsDown('D')) {
		right = 1.0f;
	}
	
	// Set speed
	if ((left + right) != 0.0f || (up + down) != 0.0f) {
		// clamp speed to max speed
		speed_ = speed_ < max_speed_ ? (speed_ + acceleration_) : max_speed_;
	}
	//speed_ = 2.0f;
	SetMovementVec({ left + right, up + down });
	position_ = position_ + movement_vector_;
	SyncSpritePosition();
}

void ZombsPlayer::Draw()
{
	sprite_.Draw();
}

void ZombsPlayer::UpdateSpriteOrientation(const float & angle)
{
	sprite_.SetAngle(angle);
}
