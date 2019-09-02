#pragma once

#include <Windows.h>
#include "Input.h"
#include "Graphics.h"

class Game {
protected:
	std::shared_ptr<Input> input_;		   // pointer to input
	std::shared_ptr<Graphics> graphics_;   // pointer to graphics
	HWND  hwnd_;						   // window handle
	bool  initialized_;					   // if game is initialized
	bool  paused_;						   // if game is paused
	HRESULT hr_;						   // standard return type 
	// Game Time Variables
	LARGE_INTEGER time_start_;			   // performance counter start value
	LARGE_INTEGER time_end_;			   // performance counter end value
	LARGE_INTEGER timer_freqency_;		   // performance counter frequency
	float frame_time_;					   // time since last frame
	float frames_per_second_;			   // hold the frames per second
	DWORD sleep_time_;					   // required sleep time to match minimum time per frame
	// constant frame rate variables
	const float target_frame_rate_ = 200.0f;						  
	const float minimum_frame_rate_ = 10.0f;						  
	const float minimum_frame_time_ = 1.0f / target_frame_rate_;	  // minimum time to pass for 1 frame
	const float maximum_frame_time_ = 1.0f / minimum_frame_rate_;	  // maximum time in 1 frame, used in calculations
	// Windows Paint Variables
	HDC hdc;
	PAINTSTRUCT ps = { 0 };
	RECT rect;
public:
	// handle window state, if window resize
	int window_width_;
	int window_height_;
	int window_center_x_;
	int window_center_y_;
public:
	Game(HWND hwnd);
	virtual ~Game();
	void Initialize(HWND hwnd);
	LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	bool HandleFrameTime();
	void Run(HWND hwnd);
	void UpdateWindowDimensions();
	virtual void Update(std::shared_ptr<Input>& input, const float& dt) = 0;
	virtual void AI() = 0;
	virtual void Collisions() = 0;
	virtual void Render() = 0;
};