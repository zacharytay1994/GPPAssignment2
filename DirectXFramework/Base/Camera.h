#pragma once

#include <DirectXMath.h>

class Sprite;
class Camera {
private:
	float camera_x_world_ = 0.0f;		 // default camera position
	float camera_y_world_ = 0.0f;		 // default camera position
public:
	static DirectX::XMMATRIX camera_translation_;
	static DirectX::XMMATRIX camera_rotation_;
public:
	Camera();
	~Camera();
	void FollowSprite(Sprite& sprite);
	float GetCameraXWorld();
	float GetCameraYWorld();
};