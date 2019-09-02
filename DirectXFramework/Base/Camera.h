#pragma once

#include <DirectXMath.h>

class Sprite;
class Camera {
private:
	float camera_x_ = 0.0f;		 // default camera position
	float camera_y_ = 0.0f;		 // default camera position
public:
	static DirectX::XMMATRIX camera_translation_;
	static DirectX::XMMATRIX camera_rotation_;
public:
	Camera();
	~Camera();
	void FollowSprite(Sprite& sprite);
};