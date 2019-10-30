#include "TestObject.h"
#include <cmath>

TestObject::TestObject(std::shared_ptr<Graphics> graphics, const std::wstring & spritename, std::shared_ptr<SpriteResources>& sr, const int& x, const int& y)
	:
	Entity(graphics, spritename, sr),
	heading_normal_(GenerateRandomHeadingNormal())
{
	world_position_ = { (float)x, (float)y };
	aabb_.radius_ = { w_radius_, h_radius_ };
	sprite_.SetWidth(50);
	sprite_.SetHeight(50);
	velocity_ = heading_normal_ * speed_;
}

void TestObject::CDraw()
{
	sprite_.Draw();
}

void TestObject::CUpdate(const float & frametime)
{
	SyncSpriteWorldPosition();
	world_position_ += heading_normal_ * speed_ * frametime;
	ClearWalls();
	SyncAABBWorldPosition();
}

void TestObject::SyncSpriteWorldPosition()
{
	sprite_.SetX((int)world_position_.x_);
	sprite_.SetY((int)world_position_.y_);
}

void TestObject::SyncAABBWorldPosition()
{
	aabb_.center_ = world_position_;
}

#define PI 3.14f
Vec2<float> TestObject::GenerateRandomHeadingNormal()
{
	// get random angle
	int angle = rand() % 360 + 0;
	Vec2<float> upright = { 0.0f,1.0f };
	float radian = angle * (PI / 180);
	float first_val = (upright.x_ * cos(radian)) + (upright.y_ * -sin(radian));
	float second_val = (upright.x_ * sin(radian)) + (upright.y_ * cos(radian));
	return { first_val, second_val };
	//return { 1.0f,0.0f };
}

void TestObject::ClearWalls()
{
	if (world_position_.x_ - w_radius_ < 0.0f) {
		world_position_.x_ = 0.0f + w_radius_;
		heading_normal_.x_ *= -1;
	}
	if (world_position_.x_ + w_radius_ > 800.0f) {
		world_position_.x_ = 800.0f - w_radius_;
		heading_normal_.x_ *= -1;
	}
	if (world_position_.y_ - h_radius_ < 0.0f) {
		world_position_.y_ = 0.0f + h_radius_;
		heading_normal_.y_ *= -1;
	}
	if (world_position_.y_ + h_radius_ > 600.0f) {
		world_position_.y_ = 600.0f - h_radius_;
		heading_normal_.y_ *= -1;
	}
}
