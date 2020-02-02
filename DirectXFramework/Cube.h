#pragma once

#include "Base/Graphics.h"
#include "Base/Surface.h"
#include "Base/Input.h"
#include "Vec3.h"
#include <memory>
#include <vector>

namespace dx = DirectX;

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
	ID3D11ShaderResourceView* srv_sprite_ = nullptr;	// pointer to sprite image resource ready for pixel shader sampling
};

class Cube {
private:
	Surface			image_resource_;			// array of color values representing an image, used to create a shader resource view to be bound to the pipeline (see CreateShaderResourceView())
	CubeData		cube_data_;				// sprite data struct as above
	std::shared_ptr<Graphics> gfx;				// graphics reference
	std::shared_ptr<Input> input;

	//// Vertex buffer ready to bind to graphics pipeline before drawing
	//Graphics::CubeVertexBuffer cvb = gfx->GetDefaultCubeVB();

	// Uninitialized sprites cannot be drawn
	bool	initialized_ = false;			 	  // set in Sprite::IntializeSprite()
	bool	visible_ = true;					  // referenced in Sprite::Draw()

public:
	Cube(std::shared_ptr<Graphics> gfx, std::shared_ptr<Input> input, const std::wstring& filename);
	~Cube();

	/*___________________________________*/
	// GET AND SET FUNCTIONS
	/*___________________________________*/
	Surface GetSurface();
	dx::XMMATRIX GetTransform(const float& dt);

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
	bool	Visible();

	/*___________________________________*/
	// MISCELLENEOUS FUNCTIONS
	/*___________________________________*/
	// Pre: initialized_ == true
	void Draw(const float& dt);
	void Update(const float& frametime);
	/*___________________________________*/
	// BACK-END FUNCTIONS
	/*___________________________________*/
	// Post: Allows Sprite::Draw() to be called
	void InitializeCube(const int& x, const int& y, const int& z, const float& scalex,
		const float& scaley, const float& scalez, const float& anglez, const float& anglex, const float& angley);
	// Pre : graphics referenced is initialized
	// Post: sprite_data_.p_srv_sprite_ filled by image_resource_, i.e. != nullptr
	void CreateShaderResourceView();
	// Update the vertex buffer
	// Invariant: vertex coordinates
	// Post     : texture (uv) coordinates
	//void UpdateCVB();
};
