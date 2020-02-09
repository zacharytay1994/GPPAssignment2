#pragma once

#include "Base/Graphics.h"
#include "Base/Surface.h"
#include "Base/Input.h"
#include "Vec3.h"
#include "ResourceLibrary.h"
#include "PhysicsMath.h"
#include <memory>
#include <vector>

namespace dx = DirectX;

enum class DrawMode {
	TexturedCube,
	TexturedCubeNormal,
	TexturedModel,
	TexturedModelNormal
};
struct CubeData {
	float	width_ = 0;									// width of cube in pixels 
	float	height_ = 0;								// height..
	float	depth_ = 0;									// depth..
	float	world_xoffset_ = 0.0f;						// normalized xoffset from origin top-left
	float	world_yoffset_ = 0.0f;						// normalized yoffset..
	float	world_zoffset_ = 0.0f;						// normalized zoffset
	float	scale_x_ = 1.0f;							// x-scale of sprite
	float	scale_y_ = 1.0f;							// y-scale..
	float	scale_z_ = 1.0f;							// z-scale..
	float	angle_z = 0.0f;								// rotation angle of sprite in radians
	float	angle_x = 0.0f;
	float	angle_y = 0.0f;
	// quaternion rotation
	QuaternionUWU rotation_ = QuaternionUWU(0.0f, 0.0f, 0.0f, 1.0f);
};

class Cube {
private:
	CubeData cube_data_;								// sprite data struct as above
	std::shared_ptr<Graphics> gfx;						// graphics reference
	std::shared_ptr<Input> input;
	std::shared_ptr<ResourceLibrary> rl_;				// holds all resource data loaded for the game
	std::string texture_key_;							// used to read into resource library
	DrawMode draw_mode_ = DrawMode::TexturedCubeNormal;
	bool physics_draw_ = false;

	// Uninitialized sprites cannot be drawn
	bool	initialized_ = false;			 			// set in Sprite::IntializeSprite()
	bool	visible_ = true;							// referenced in Sprite::Draw()

public:
	Cube(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, const std::string& filename, const std::shared_ptr<ResourceLibrary> rl);
	~Cube();

	/*___________________________________*/
	// GET AND SET FUNCTIONS
	/*___________________________________*/
	dx::XMMATRIX GetTransform();
	dx::XMMATRIX GetModelTransform();
	dx::XMMATRIX GetQuaternionTransform();

	/*___________________________________*/
	// SPRITE DATA
	/*___________________________________*/
	void	SetX(const float& x);
	void	SetY(const float& y);
	void	SetZ(const float& z);
	void	SetWidth(const int& width);
	void	SetHeight(const int& height);
	void	SetDepth(const int& depth);
	void	SetScaleX(const float& scalex);
	void	SetScaleY(const float& scaley);
	void	SetScaleZ(const float& scalez);
	void	SetAngleZ(const float& angle);
	void	SetAngleX(const float& angle);
	void	SetAngleY(const float& angle);
	void	SetAngleZDeg(const float& angle);
	void	SetAngleXDeg(const float& angle);
	void	SetAngleYDeg(const float& angle);
	void	SetQuatRotation(const QuaternionUWU& q);
	float	GetX();
	float	GetY();
	float	GetZ();
	int		GetWidth();
	int		GetHeight();
	int		GetDepth();
	float	GetScaleX();
	float	GetScaleY();
	float	GetScaleZ();
	float	GetAngleZ();
	float	GetAngleX();
	float	GetAngleY();

public:
	void	SetVisible(const bool& visible);
	void	SetPhysicsDraw(const bool& b);
	void	SetDrawMode(const int& drawmode);
	bool	Visible();

	/*___________________________________*/
	// MISCELLENEOUS FUNCTIONS
	/*___________________________________*/
	// Pre: initialized_ == true
	void Draw();
	void DrawWithQuaternion();
	void DrawModel();
	void DrawModelNorm();
	void HandleDraw();
	void Update(const float& frametime);
	/*___________________________________*/
	// BACK-END FUNCTIONS
	/*___________________________________*/
	// Post: Allows Sprite::Draw() to be called
	void InitializeCube(const int& x, const int& y, const int& z, const float& scalex,
		const float& scaley, const float& scalez, const float& anglez, const float& anglex, const float& angley);
};
