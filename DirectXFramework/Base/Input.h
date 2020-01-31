#pragma once

#include <DirectXMath.h>
#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <vector>

namespace inputns {
	// Size of keys array
	const int kKeysArrayLength = 256;
	// Values for clear(), bit flag
	const UCHAR kKeysDown = 1;
	const UCHAR kKeysPressed = 2;
	const UCHAR kMouse = 4;
	const UCHAR kTextIn = 8;
	const UCHAR kKeysMouseText = kKeysDown + kKeysPressed + kMouse + kTextIn;
}

class Input {
private:
	bool keys_down_[inputns::kKeysArrayLength];		// true if specified key is down, false otherwise
	bool keys_pressed_[inputns::kKeysArrayLength];	// true if specified key was pressed in latest frame, false otherwise, reset to false every frame
	std::string text_in_;							// user entered text
	char char_in_;									// last character entered
	bool new_line_;									// true on start of new line
	int  mouse_x_, mouse_y_;						// mouse screen coordinates
	int  mouse_raw_x_, mouse_raw_y_;				// high definition mouse data
	RAWINPUTDEVICE rid_[1];							// for high definition mouse
	bool mouse_captured_;							// true is mouse captured
	bool mouse_left_button_;						// true if left mouse button down
	bool mouse_middle_button_;						// true if middle mouse button down
	bool mouse_right_button_;						// true if right mouse button down
	bool mouse_x1_button_;							// true if x1 mouse button down
	bool mouse_x2_button_;							// true if x2 mouse button down
	bool state_change_;

	// Camera Variables
	std::vector<char> raw_buffer_;
	float cam_yaw_ = 0.0f;
	float cam_pitch_ = 0.0f;
	float cam_x_ = 0.0f;
	float cam_y_ = 0.0f;
	float cam_z_ = 0.0f;
	float camera_translate_speed_ = 5.0f;

	bool camera_engaged_ = true;
public:
	Input();										// default constructor
	virtual ~Input();								// destructor
	void Initialize(HWND hwnd, bool capture);		// initalize mouse input
	void Update(const float& dt);
	// Keys & Text
	void SetKeyDown(WPARAM wparam);					// set key state of VIRTUAL KEY in keys_down_ and keys_pressed buffer
	void SetKeyUp(WPARAM wparam);					// set key state of VIRTUAL KEY in keys_down_ buffer
	void SetKeyIn(WPARAM wparam);					// set char_in_ to latest char entered in text_in_ string
	bool KeyIsDown(UCHAR vkey) const;				// return state of VIRTUAL KEY in keys_down_ buffer
	bool KeyWasPressed(UCHAR vkey) const;			// return if a specified VIRTUAL KEY was pressed in the most recent frame
	bool AnyKeyWasPressed() const;					// return if any VIRTUAL KEY was pressed in the most recent frame
	void SetKeyPressedFalse(UCHAR vkey);			// changes the specified VIRTUAL KEY flag in keys_pressed_ buffer to false
	void ClearBuffer(UCHAR buffers);				// clear specified combination of buffer using bit flags, e.g. kKeysDown | kKeysPressed is 0011
	void ClearAll();								// clears all input data buffers i.e. ClearBuffer(kKeysMouseText)
	void ClearTextIn();								// clear text input buffer, i.e. text_in_
	std::string GetTextIN();						// return text_in_
	char GetCharIn();								// return last character entered, i.e. char_in_
	// Mouse
	void FillMousePosition(LPARAM lparam);			// reads and sets mouse_x_ and mouse_y_
	void FillMouseRawPosition(LPARAM lparam);		// read and sets mouse_raw_x_ and mouse_raw_y_
	void SetMouseLeftButton(bool flag);				// set left mouse button state
	void SetMouseMiddleButton(bool flag);			// set middle mouse button state
	void SetMouseRightButton(bool flag);			// set right mouse button state
	void SetMouseXButton(WPARAM wparam);			// set x1 and x2 mouse button state
	int  GetMouseX();								// return mouse x coordinate
	int  GetMouseY();								// return mouse y coordinate
	int  GetMouseRawX();							// return mouse raw x coordinate, for high definition mice
	int  GetMouseRawY();							// return mouse raw y coordinate, for high definition mice
	bool GetMouseLeftButton();						// return left mouse button state
	bool GetMouseMiddleButton();					// return middle mouse button state
	bool GetMouseRightButton();						// return right mouse button state
	bool GetMouseX1Button();						// return x1 mouse button state
	bool GetMouseX2Button();						// return x2 mouse button state
	// Miscelleneous Functions
	bool StateChanged();							// returns true if there is a change in buffer state in current frame
	void TestKeys(HDC hdc, RECT rect);				// visible interface to test virtual key message processing
	void EndFrame();								// handles all end of frame operations for input

	// Camera Functions
	void ResetCamera();
	DirectX::XMMATRIX GetCameraMatrix(const float& dt);
	DirectX::XMMATRIX GetInverseCameraRotation();
	void TranslateCamera(DirectX::XMFLOAT3 translation, const float& dt);
	void TranslateNoY(DirectX::XMFLOAT3 translation, const float& dt);
	void TranslateNoRotation(DirectX::XMFLOAT3 translation, const float& dt);
};