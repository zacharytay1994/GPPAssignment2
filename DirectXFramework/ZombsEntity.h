#pragma once

#include "Base/Sprite.h"
#include "Base/Graphics.h"
#include "ZombConstants.h"
#include "Base/Input.h"
#include <string>

class ZombsEntity {
protected:
	float x_ = 0.0f; 		  // default initialize world center
	float y_ = 0.0f;		  // default initialize world center
	Sprite sprite_;
	Vector2D position_ = { x_, y_ };

	// Travelling variables
	float speed_ = 0.0f;
	float acceleration_ = 0.0f;
	Vector2D horizontal_vector_ = { 0.0f, 0.0f };
	Vector2D vertical_vector_ = { 0.0f, 0.0f };
	Vector2D movement_vector_ = (horizontal_vector_ + vertical_vector_).Normalized() * speed_;
public:
	ZombsEntity(std::shared_ptr<Graphics>& graphics, const std::wstring& spriteresource, const int& width, const int& height);
	~ZombsEntity();
	void SetHeadingVec(const Vector2D& headingvector);
	virtual void Update(std::shared_ptr<Input>& input, const float& dt) = 0;
	virtual void Draw() = 0;

	// Get Set
	void SetHorizontalVec(const Vector2D& vec);
	void SetVerticalVec(const Vector2D& vec);
	void SetMovementVec(const Vector2D& vec);
	void SyncSpritePosition();
	void SetSpeed(const float& speed);
};