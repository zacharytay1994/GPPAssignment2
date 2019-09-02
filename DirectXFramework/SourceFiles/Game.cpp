#include "../Base/Game.h"
#pragma comment(lib, "winmm.lib")

Game::Game(HWND hwnd)
	:
	input_(std::make_shared<Input>()),
	graphics_(std::make_shared<Graphics>())
{
	Initialize(hwnd);
}

Game::~Game()
{
}

void Game::Initialize(HWND hwnd)
{
	// set window handle
	hwnd_ = hwnd;
	// intialize input, true=mousecapture, false=nomousecapture
	input_->Initialize(hwnd, false);
	graphics_->Initialize(hwnd);
	graphics_->InitSpritePipeline();
	// set up high resolution timer, always succeeds on systems running Windows XP or later
	// retrieves current performance counter frequency in counts per second and stores it in timer_frequency_
	QueryPerformanceFrequency(&timer_freqency_);
	// sets time_start_ to current performance counter value
	QueryPerformanceCounter(&time_start_);
	// set window dimensions
	window_width_ = Graphics::viewport_width_;
	window_height_ = Graphics::viewport_height_;
	window_center_x_ = window_width_ / 2;
	window_center_y_ = window_height_ / 2;
	// flag initialized_
	initialized_ = true;
}

LRESULT Game::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case WM_KEYDOWN: case WM_SYSKEYDOWN:
		input_->SetKeyDown(wparam);
		return 0;
	case WM_KEYUP: case WM_SYSKEYUP:
		input_->SetKeyUp(wparam);
		return 0;
	case WM_CHAR:			// WM_CHAR message only generated if: any character key, backspace (tested doesn't), enter, escape, shift+enter, tab, is pressed.
		switch (wparam) {
		// Additional Cases
		case VK_BACK:		// backspace
		case VK_TAB:		// tab
		case 0x0A:			// linefeed
		case VK_RETURN:		// carriage return
		case VK_ESCAPE:		// escape
			// makes the "dulung sound"
			MessageBeep((UINT)-1);
			// stores pressed key character in input_::char_in_
			input_->SetKeyIn(wparam);
			return 0;
		default:
			input_->SetKeyIn(wparam);
			//InvalidateRect(hwnd, nullptr, TRUE); // adds a WM_PAINT message to message queue
			return 0;
		}
	case WM_MOUSEMOVE:
		input_->FillMousePosition(lparam);
		return 0;
	case WM_INPUT:
		input_->FillMouseRawPosition(lparam);
		return 0;
	case WM_LBUTTONDOWN:
		input_->SetMouseLeftButton(true);
		input_->FillMousePosition(lparam);
		return 0;
	case WM_LBUTTONUP:
		input_->SetMouseLeftButton(false);
		input_->FillMousePosition(lparam);
		return 0;
	case WM_MBUTTONDOWN:
		input_->SetMouseMiddleButton(true);
		input_->FillMousePosition(lparam);
		return 0;
	case WM_MBUTTONUP:
		input_->SetMouseMiddleButton(false);
		input_->FillMousePosition(lparam);
		return 0;
	case WM_RBUTTONDOWN:
		input_->SetMouseRightButton(true);
		input_->FillMousePosition(lparam);
		return 0;
	case WM_RBUTTONUP:
		input_->SetMouseRightButton(false);
		input_->FillMousePosition(lparam);
		return 0;
	case WM_XBUTTONDOWN: case WM_XBUTTONUP:
		input_->SetMouseXButton(wparam);
		input_->FillMousePosition(lparam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(42069);
		return 0;
	case WM_PAINT: // gets flagged after InvalidateRect() call
	// window client coordinates to be filled, typedef struct tagRect { LONG left; LONG top; LONG right; LONG bottom; }
	// prepares window for painting, and gets handle to device context
		hdc = BeginPaint(hwnd, &ps);
		// filled rect with client window coordinate information
		GetClientRect(hwnd, &rect);
		// test all key inputs
		//input_->TestKeys(hdc, rect);
		EndPaint(hwnd, &ps);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

bool Game::HandleFrameTime()
{
	// sets time_end_ to current performance counter value
	QueryPerformanceCounter(&time_end_);
	// calculate time since last frame, counter time last frame stored in time_start_
	// QuadPart contains a 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC)
	frame_time_ = (float)(time_end_.QuadPart - time_start_.QuadPart) / (float)timer_freqency_.QuadPart;

	// if not enough time has passed in current frame
	if (frame_time_ < minimum_frame_time_) {
		// calculate and set sleep time
		sleep_time_ = (DWORD)((minimum_frame_time_ - frame_time_) * 1000);
		timeBeginPeriod(1);		// request 1ms resolution for windows timer
		Sleep(sleep_time_);		// release cpu for sleep_time_
		timeEndPeriod(1);		// end 1ms timer resolution
		return false;
	}
	
	if (frame_time_ > 0.0f) {
		frames_per_second_ = (frames_per_second_ * 0.99f) + (0.01f / frame_time_); // 
	}

	if (frame_time_ > maximum_frame_time_) {
		frame_time_ = maximum_frame_time_;
	}

	time_start_ = time_end_;
	return true;
}

void Game::Run(HWND hwnd)
{
	// run clock
	if (!HandleFrameTime()) { return; }

	graphics_->ClearBuffer();

	// process components
	if (!paused_) {
		Update(input_, frame_time_);
		AI();
		Collisions();
		Render();
	}

	// clear keys_pressed_ buffer of input
	input_->EndFrame();
	// present frame
	graphics_->EndFrame();
}

void Game::UpdateWindowDimensions()
{
	if (input_->KeyWasPressed('U')) {
		RECT rect;
		if (GetWindowRect(hwnd_, &rect)) {
			window_width_ = rect.right - rect.left;
			window_height_ = rect.bottom - rect.top;
		}
	}
}
