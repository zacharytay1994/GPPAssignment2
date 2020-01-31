#include "../Base/Input.h"

#include <string>
#include <sstream>

// makes sure key code is within buffer range
inline bool ValidKeyCode(UCHAR vkey) { return vkey < inputns::kKeysArrayLength; }

Input::Input()
{
	// set keys_down_ buffer to false
	for (bool &b : keys_down_) {
		b = false;
	}
	// set keys_pressed_ buffer to false
	for (bool &b : keys_pressed_) {
		b = false;
	}
	// default all member variables
	new_line_ = true;
	text_in_ = "";
	char_in_ = 0;
	mouse_x_ = 0;
	mouse_y_ = 0;
	mouse_raw_x_ = 0;
	mouse_raw_y_ = 0;
	mouse_left_button_ = false;
	mouse_middle_button_ = false;
	mouse_right_button_ = false;
	mouse_x1_button_ = false;
	mouse_x2_button_ = false;
	state_change_ = false;
}

Input::~Input()
{
	// release mouse
	if (mouse_captured_) {
		ReleaseCapture();
	}
}

void Input::Initialize(HWND hwnd, bool capture)
{
	mouse_captured_ = capture;
	// set capture mouse input over application window
	if (mouse_captured_) {
		SetCapture(hwnd);
	}
}

void Input::Update(const float& dt)
{
	ResetCamera();
	if (camera_engaged_) {
		if (KeyIsDown('W')) {
			TranslateCamera({0.0f, 0.0f, 1.0f}, dt);
		}
		if (KeyIsDown('S')) {
			TranslateCamera({ 0.0f, 0.0f, -1.0f }, dt);
		}
		if (KeyIsDown('A')) {
			TranslateCamera({ -1.0f, 0.0f, 0.0f }, dt);
		}
		if (KeyIsDown('D')) {
			TranslateCamera({ 1.0f, 0.0f, 0.0f }, dt);
		}
		if (KeyIsDown(VK_SHIFT)) {
			TranslateCamera({ 0.0f, -1.0f, 0.0f }, dt);
		}
		if (KeyIsDown(VK_SPACE)) {
			TranslateCamera({ 0.0f, 1.0f, 0.0f }, dt);
		}
	}
}

void Input::SetKeyDown(WPARAM wparam)
{
	// make sure key code is within buffer range
	if (ValidKeyCode(wparam)) {
		state_change_ = true;
		keys_down_[wparam] = true;
		keys_pressed_[wparam] = true;
		// additional check for keys with left and right
		switch (wparam) {
		case VK_SHIFT:
			// GetKeyState returns a 8 bit value representing individual key state, 
			// if high-order bit (leftmost bit) == 1, key is down, if == 0, key is up
			// e.g. GetKeyState(VK_LSHIFT) returns 10101100, 1010110 & (masked by) 1000000 is 1000000, which = 128, >0 = true;
			if (GetKeyState(VK_LSHIFT) & 128) {
				keys_down_[VK_LSHIFT] = true;
			}
			if (GetKeyState(VK_RSHIFT) & 128) {
				keys_down_[VK_RSHIFT] = true;
			}
			break;
		case VK_CONTROL:
			if (GetKeyState(VK_LCONTROL) & 128) {
				keys_down_[VK_LCONTROL] = true;
			}
			if (GetKeyState(VK_RCONTROL) & 128) {
				keys_down_[VK_RCONTROL] = true;
			}
			break;
		}
	}
}

void Input::SetKeyUp(WPARAM wparam)
{
	if (ValidKeyCode(wparam)) {
		state_change_ = true;
		keys_down_[wparam] = false;
		switch (wparam) {
		// weird bug here, if both shifts are down, releasing one does not generate a message?
		// control works fine with the same method, unsure whats wrong
		case VK_SHIFT:
			if ((GetKeyState(VK_LSHIFT) & 128) == 0) {
				keys_down_[VK_LSHIFT] = false;
			}
			if ((GetKeyState(VK_RSHIFT) & 128) == 0) {
				keys_down_[VK_RSHIFT] = false;
			}
			break;
		case VK_CONTROL:
			if ((GetKeyState(VK_LCONTROL) & 128) == 0) {
				keys_down_[VK_LCONTROL] = false;
			}
			if ((GetKeyState(VK_RCONTROL) & 128) == 0) {
				keys_down_[VK_RCONTROL] = false;
			}
			// only release main control if both sides are released
			keys_down_[wparam] = keys_down_[VK_LCONTROL] || keys_down_[VK_RCONTROL] ? true : false;
			break;
		}
	}
}

void Input::SetKeyIn(WPARAM wparam)
{
	// if start of new line
	if (new_line_) {
		text_in_.clear();
		new_line_ = false;
	}
	// if backspace
	if ((char)wparam == '\b') {
		// if characters exist
		if (text_in_.length() > 0) {
			// erase last character of string
			text_in_.erase(text_in_.size() - 1);
		}
	}
	else {
		// add character to string
		text_in_ += (char)wparam;
		// update last input character
		char_in_ = (char)wparam;
	}
	// if carriage return
	if ((char)wparam == '\r') {
		new_line_ = true;
	}
}

bool Input::KeyIsDown(UCHAR vkey) const
{
	if (ValidKeyCode(vkey)) {
		return keys_down_[vkey];
	}
	return false;
}

bool Input::KeyWasPressed(UCHAR vkey) const
{
	if (ValidKeyCode(vkey)) {
		return keys_pressed_[vkey];
	}
	return false;
}

bool Input::AnyKeyWasPressed() const
{
	for (bool b : keys_pressed_) {
		if (b) {
			return true;
		}
	}
	return false;
}

void Input::SetKeyPressedFalse(UCHAR vkey)
{
	if (ValidKeyCode(vkey)) {
		keys_pressed_[vkey] = false;
	}
}

void Input::ClearBuffer(UCHAR buffers)
{
	if (buffers & inputns::kKeysDown) {
		for (bool &b : keys_down_) {
			b = false;
		}
	}
	if (buffers & inputns::kKeysPressed) {
		for (bool &b : keys_pressed_) {
			b = false;
		}
	}
	if (buffers & inputns::kMouse) {
		mouse_x_ = 0;
		mouse_y_ = 0;
		mouse_raw_x_ = 0;
		mouse_raw_y_ = 0;
	}
	if (buffers & inputns::kTextIn) {
		ClearTextIn();
	}
}

void Input::ClearAll()
{
	ClearBuffer(inputns::kKeysMouseText);
}

void Input::ClearTextIn()
{
	text_in_.clear();
}

std::string Input::GetTextIN()
{
	return text_in_;
}

char Input::GetCharIn()
{
	return char_in_;
}

void Input::FillMousePosition(LPARAM lparam)
{
	mouse_x_ = GET_X_LPARAM(lparam);
	mouse_y_ = GET_Y_LPARAM(lparam);
}

void Input::FillMouseRawPosition(LPARAM lparam)
{
	//// data size
	//UINT dw_size = 40;
	//// data
	//static BYTE lpb[40];

	//GetRawInputData((HRAWINPUT)lparam, RID_INPUT,
	//	lpb, &dw_size, sizeof(RAWINPUTHEADER));

	//RAWINPUT* raw = (RAWINPUT*)lpb;

	//if (raw->header.dwType == RIM_TYPEMOUSE) {
	//	mouse_raw_x_ = raw->data.mouse.lLastX;
	//	mouse_raw_y_ = raw->data.mouse.lLastY;
	//}
	UINT size;
	// frst gt the size of the input data
	if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam),
		RID_INPUT,
		nullptr,
		&size,
		sizeof(RAWINPUTHEADER)) == -1) {
		return;
	}
	raw_buffer_.resize(size);
	// read in the input data
	if (GetRawInputData(
		reinterpret_cast<HRAWINPUT>(lparam),
		RID_INPUT,
		raw_buffer_.data(),
		&size,
		sizeof(RAWINPUTHEADER)) != size) {
		return;
	}
	auto& raw_input = reinterpret_cast<const RAWINPUT&>(*raw_buffer_.data());
	if (raw_input.header.dwType == RIM_TYPEMOUSE &&
		(raw_input.data.mouse.lLastX != 0 || raw_input.data.mouse.lLastY != 0)) {
		mouse_raw_x_ = raw_input.data.mouse.lLastX;
		mouse_raw_y_ = raw_input.data.mouse.lLastY;
	}
}

void Input::SetMouseLeftButton(bool flag)
{
	mouse_left_button_ = flag;
}

void Input::SetMouseMiddleButton(bool flag)
{
	mouse_middle_button_ = flag;
}

void Input::SetMouseRightButton(bool flag)
{
	mouse_right_button_ = flag;
}

void Input::SetMouseXButton(WPARAM wparam)
{
	mouse_x1_button_ = (wparam & MK_XBUTTON1) ? true : false;
	mouse_x2_button_ = (wparam & MK_XBUTTON2) ? true : false;
}

int Input::GetMouseX()
{
	return mouse_x_;
}

int Input::GetMouseY()
{
	return mouse_y_;
}

int Input::GetMouseRawX()
{
	return mouse_raw_x_;
}

int Input::GetMouseRawY()
{
	return mouse_raw_y_;
}

bool Input::GetMouseLeftButton()
{
	return mouse_left_button_;
}

bool Input::GetMouseMiddleButton()
{
	return mouse_middle_button_;
}

bool Input::GetMouseRightButton()
{
	return mouse_right_button_;
}

bool Input::GetMouseX1Button()
{
	return mouse_x1_button_;
}

bool Input::GetMouseX2Button()
{
	return mouse_x2_button_;
}

bool Input::StateChanged()
{
	return state_change_;
}

void Input::TestKeys(HDC hdc, RECT rect)
{
	TCHAR output[16];
	int char_count = 0;
	const int offset_y = 90;
	int padding_y_plus = 0;
	int offset_x = 60;
	for (bool b : keys_down_) {
		if (char_count > 15) {
			TextOut(hdc, rect.right/2 - offset_x, rect.bottom/2 - offset_y + padding_y_plus, output, ARRAYSIZE(output));
			padding_y_plus += 15;
			char_count = 0;
		}
		if (b) {
			output[char_count] = 'T';
		}
		else {
			output[char_count] = 'F';
		}
		char_count++;
	}
	TextOut(hdc, 0, 0, &char_in_, 1);
}

void Input::EndFrame()
{
	ClearBuffer(inputns::kKeysPressed);
	state_change_ = false;
}

void Input::ResetCamera()
{
	mouse_raw_x_ = 0;
	mouse_raw_y_ = 0;
}

DirectX::XMMATRIX Input::GetCameraMatrix()
{
	// clamping x
	cam_yaw_ = cam_yaw_ < 6.284f ? cam_yaw_ + mouse_raw_x_ * 0.002f : 0.0f;
	// clamping y
	float temp = (cam_pitch_ + mouse_raw_y_ * 0.002f);
	temp = temp < -1.571f * 0.9f ? -1.571 * 0.9f : temp;
	temp = temp > 1.571f * 0.9f ? 1.571f * 0.9f : temp;
	cam_pitch_ = temp;
	cam_pitch_ = cam_pitch_ + mouse_raw_y_ * 0.002f;
	using namespace DirectX;
	DirectX::XMVECTOR forward_base_vector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR look_vector = DirectX::XMVector3Transform(forward_base_vector,
		DirectX::XMMatrixRotationRollPitchYaw(cam_pitch_, cam_yaw_, 0.0f));
	DirectX::XMVECTOR cam_position = DirectX::XMVectorSet(cam_x_, cam_y_, cam_z_, 0.0f);
	DirectX::XMVECTOR cam_target = cam_position + look_vector;
	return DirectX::XMMatrixLookAtLH(cam_position, cam_target, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

DirectX::XMMATRIX Input::GetInverseCameraRotation()
{
	cam_yaw_ = cam_yaw_ < 6.284f ? cam_yaw_ + mouse_raw_x_ * 0.002f : 0.0f;
	float temp = (cam_pitch_ + mouse_raw_y_ * 0.002f);
	temp = temp < -1.571f * 0.9f ? -1.571 * 0.9f : temp;
	temp = temp > 1.571f * 0.9f ? 1.571f * 0.9f : temp;
	cam_pitch_ = temp;
	return DirectX::XMMatrixRotationRollPitchYaw(-cam_pitch_, -cam_yaw_, 0.0f);
}

void Input::TranslateCamera(DirectX::XMFLOAT3 translation, const float& dt)
{
	using namespace DirectX;
	XMStoreFloat3(&translation, XMVector3Transform(
		XMLoadFloat3(&translation),
		XMMatrixRotationRollPitchYaw(cam_pitch_, cam_yaw_, 0.0f) *
		XMMatrixScaling(camera_translate_speed_ * dt, camera_translate_speed_ * dt, camera_translate_speed_ * dt)
	));
	cam_x_ += translation.x;
	cam_y_ += translation.y;
	cam_z_ += translation.z;
}
