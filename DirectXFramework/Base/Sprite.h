#pragma once

#include "Graphics.h"
#include "Surface.h"
#include <memory>
#include <vector>

namespace dx = DirectX;

struct SpriteData {
	int		width_;								// width of sprite in pixels 
	int		height_;							// height..
	float	world_xoffset_;						// normalized xoffset from origin top-left
	float	world_yoffset_;						// normalized yoffset..
	int		screen_xoffset_;					// pixel x-offset..
	int		screen_yoffset_;					// pixel y-offset..
	float	scale_x_;							// x-scale of sprite
	float	scale_y_;							// y-scale..
	float	scale_dimension_x_;					// x-scale to convert dimensions from normalized to screen coordinates
	float	scale_dimension_y_;					// y-scale
	float	angle_;								// rotation angle of sprite in radians
	RECT	rect_;								// ---
	ID3D11ShaderResourceView* srv_sprite_;		// pointer to sprite image resource ready for pixel shader sampling
	bool	flip_horizontal_;					// ---
	bool	flip_vertical_;						// ---
};

// Texture coordinate structs for pixel shader processing
struct	Coord { float x; float y; };
struct	TexCoord { Coord top_left; Coord top_right; Coord bottom_left; Coord bottom_right; };

class Sprite {
private:
	Surface			image_resource_;			// array of color values representing an image, used to create a shader resource view to be bound to the pipeline (see CreateShaderResourceView())
	SpriteData		sprite_data_;				// sprite data struct as above
	std::shared_ptr<Graphics> gfx;				// graphics reference

	// Stuff for sampling frames in sprites with multiple frames
	TexCoord tc_ = { {0.0f,0.0f},{1.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f} };
	// Vertex buffer ready to bind to graphics pipeline before drawing
	Graphics::VertexBuffer vb = gfx->GetDefaultVB();

	// Uninitialized sprites cannot be drawn
	bool	initialized_ = false;			 	  // set in Sprite::IntializeSprite()
	bool	visible_ = true;					  // referenced in Sprite::Draw()

	/*___________________________________*/
	// ANIMATION VARIABLES
	/*___________________________________*/
	int		sprite_sheet_columns_	= 1;		  // default 1 column i.e. 1 frame
	int		sprite_sheet_rows_		= 1;		  // default
	float	unit_x_					= 1.0f;		  // default
	float	unit_y_					= 1.0f;		  // default
	int		start_frame_			= 0;		  // default == end_frame_ i.e. no animation
	int		end_frame_				= 0;		  // default
	int		current_frame_			= 0;		  // default
	float	frame_delay_			= 1.0f;		  // default i.e. 1 second
	float	animation_timer_		= 0.0f;		  // default
	bool	loop_					= true;		  // default
	bool	animation_complete_		= false;	  // default

public:
	Sprite(std::shared_ptr<Graphics> gfx, const std::wstring& filename,
		const int& width, const int& height);
	~Sprite();

	/*___________________________________*/
	// GET AND SET FUNCTIONS
	/*___________________________________*/
	Surface GetSurface();
	dx::XMMATRIX GetTransform();

	/*___________________________________*/
	// SPRITE DATA
	/*___________________________________*/
	void	SetX(const int& x);
	void	SetY(const int& y);
	void	SetWidth(const int& width);
	void	SetHeight(const int& height);
	void	SetScaleX(const float& scalex);
	void	SetScaleY(const float& scaley);
	void	SetAngle(const float& angle);
	void	FlipHorizontal(const bool& flip);	// flips sprite horizontally if called with true, default if false
	void	FlipVertical(const bool& flip);		// flips do not stack, i.e. 2 flip trues do not result in original flip orientation
	int		GetX();
	int		GetY();
	int		GetWidth();
	int		GetHeight();
	float	GetScaleX();
	float	GetScaleY();
	float	GetAngle();

	/*___________________________________*/
	// ANIMATION DATA
	/*___________________________________*/
private:
	void	SetColumns(const int& cols);
	void	SetRows(const int& rows);
public:
	void	SetCurrentFrame(const int& frame);
	void	SetFrameDelay(const float& framedelay);
	void	SetLoop(const bool& loop);
	void	SetVisible(const bool& visible);
	int		GetCurrentFrame();
	bool	Loop();
	bool	Visible();

	/*___________________________________*/
	// MISCELLENEOUS FUNCTIONS
	/*___________________________________*/
	// Pre: initialized_ == true
	void Draw();
	void Update(const float& frametime);
	/*___________________________________*/
	// BACK-END FUNCTIONS
	/*___________________________________*/
	// Post: Allows Sprite::Draw() to be called
	void InitializeSprite(const int& x, const int& y, const float& scalex,
		const float& scaley, const float& angle);
	// Post: Allows for sprite frame transitions, as long as startframe - endframe > 0
	void InitializeAnimation(const int& cols, const int& rows, const int& startframe,
		const int& endframe, const float& framedelay, const bool& loop);
	// Pre : graphics referenced is initialized
	// Post: sprite_data_.p_srv_sprite_ filled by image_resource_, i.e. != nullptr
	void CreateShaderResourceView();
	// Calculates and sets scale_dimension_x_ and y_
	void DimensionsToScale();
	// Update the vertex buffer
	// Invariant: vertex coordinates
	// Post     : texture (uv) coordinates
	void UpdateVB();
	// Calculates and returns corresponding normalized texture coordinate with frame number
	TexCoord FrameToTexcoord(const int& frame);
};