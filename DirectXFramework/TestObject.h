#pragma once
#include "Vec2.h"
#include "Entity.h"

class TestObject : public Entity {
private:
	struct AABB {
		Vec2<float> center_ = { 0.0f,0.0f };
		Vec2<float> radius_ = { 0.0f,0.0f };
		bool is_static_ = false;
		bool IsOverlapping(const AABB& aabb) {
			// distance betweem
			Vec2<float> between = center_ - aabb.center_;
			// sum radius
			Vec2<float> sum_radius = radius_ + aabb.radius_;
			return ((abs(between.x_) > sum_radius.x_) && (abs(between.y_) > sum_radius.y_));
		}
	};

	float w_radius_ = 25.0f;
	float h_radius_ = 25.0f;
	float speed_ = 500.0f;
	Vec2<float> heading_normal_;
	AABB aabb_;
public:
	Vec2<float> velocity_ = { 0.0f, 0.0f };

public:
	TestObject(std::shared_ptr<Graphics> graphics, const std::wstring& spritename, std::shared_ptr<SpriteResources>& sr, const int& x, const int& y);
	void CDraw() override;
	void CUpdate(const float& frametime) override;
	void SyncSpriteWorldPosition();
	void SyncAABBWorldPosition();
	Vec2<float> GenerateRandomHeadingNormal();
	void ClearWalls();
};