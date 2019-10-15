// for detecting memory leaks
#define _CRTDBG_MAP_ALLOC
#define WIN32_LEAN_AND_MEAN
#include <stdlib.h>
#include <crtdbg.h>

#include <Windows.h>
#include <string>
#include <sstream>
#include "../Base/GameChild.h"
#include "../TheGame.h"
#include "../Base/GDIPlusManager.h"

// Pointer to a game instance
std::unique_ptr<TheGame> game_;

// Function Prototypes
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);			   // defines custom windows procedure
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);		   // creates window and handles message loop, continues looping if not WM_QUIT
bool AnotherInstance();										   // creates mutex to check for multiple instances

// Custom windows procedure
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	// if game has been initialized
	if (game_) {
		return (game_->HandleMessage(hwnd, msg, wParam, lParam));
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

// windows entry point
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevIntance,
	LPSTR lpCmdLine,
	int nCmdShow) {

	// Check for multiple instances
	if (AnotherInstance()) {
		return false;
	}

	// Start up gdiplus for image loading
	GDIPlusManager gdiplus_manager;

	/*________________________________________
	REGISTER WINDOW CLASS
	---------------------
	Tip : Read the msdn for all possible configurations
	WNDCLASSEX : 
	https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
	________________________________________ */

	// 1. Create window struct with default 0
	WNDCLASSEX wce = {0};
	LPCSTR wndClassName = "anyname"; // 32-bit const char * type
	// 2. Configure struct params
	wce.cbSize = sizeof(wce);
	wce.style = CS_HREDRAW | CS_VREDRAW;
	wce.lpfnWndProc = WinProc;
	wce.cbClsExtra = 0;
	wce.cbWndExtra = 0;
	wce.hInstance = hInstance;
	wce.hIcon = nullptr; // Default icon if null
	wce.hCursor = nullptr; // Default cursor if null
	wce.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // DirectX fills background, not windows
	wce.lpszMenuName = nullptr;
	wce.lpszClassName = wndClassName;
	wce.hIconSm = nullptr;
	// 3. Register the window class
	// RegisterClassEx returns 0 on error, takes a pointer to the struct
	// we wish to register.
	if (RegisterClassEx(&wce) == 0) {
		return false;
	}

	/*________________________________________
	CREATE REGISTERED WINDOW CLASS
	------------------------------
	CreateWindowExA (or CreateWindow) :
	https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
	________________________________________ */

	// Some window variables
	const int wndX = 100; // x-coord of upper left corner
	const int wndY = 100; // y-coord of upper left corner
	const int wndWidth = 800; // in pixels
	const int wndHeight = 600;
	const int wndBotX = wndX + wndWidth;
	const int wndLeftY = wndY + wndHeight;
	// Adjust window rect based on client region
	RECT rect = { wndX,wndY,wndBotX,wndLeftY };
	if (!AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, false)) {
		return false;
	};
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	// 1. Create window and store its handle (unique identifier)
	HWND hwnd = CreateWindowEx(
		0, // extended styles not needed, 0
		wndClassName,
		"any window name here", // just the name/title of the window, not to be confused with class name
		WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, // base styles
		rect.left,
		rect.top,
		width,
		height,
		nullptr, // no parent window
		nullptr, // no menu
		hInstance, // handle to application instance
		nullptr // no window parameters
	);

	if (!hwnd) { return false; } // if window creation not successful, hwnd is 0, return false

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	/*________________________________________
	FILL POINTER WITH INSTANTIATED GAME OBJECT 
	AND INITIALIZE
	________________________________________ */
	game_ = std::make_unique<TheGame>(hwnd);
	//game_->Initialize(hwnd);

	/*________________________________________
	HANDLE WINDOW MESSAGES
	PeekMessage() :
	https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-peekmessagea
	________________________________________ */

	// Create msg struct, type LPMSG
	MSG msg;
	int done = 0;
	std::wstringstream ss;
	// Message loop
	while (!done) {
		// when window handle is null, PeekMessage() retrieves messages for any window that belongs to the current thread
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			// check if quit message has been processed
			ss << msg.message << std::endl;
			OutputDebugStringW(ss.str().c_str());
			ss.clear();
			if (msg.message == WM_QUIT) {
				done = 1;
			}
			// TranslateMessage() produces WM_CHAR messages only for keys that are mapped to ASCII characters by the keyboard driver.
			TranslateMessage(&msg);
			// Dispatches message to a window procedure, our custom WinProc() in this case
			DispatchMessage(&msg);
		}
		else {
			game_->Run(hwnd);
		}
	}
	// if quit message found, window gets destroyed.
	// after executing custom WinProc for the last time and calling PostQuitMessage(val),
	// PostQuitMesage puts a WM_QUIT message on the message queue. wParam of WM_QUIT msg is val.
	return msg.wParam;
}

bool AnotherInstance() {
	HANDLE our_mutex;

	// Attempt to create a mutex using out unique string
	our_mutex = CreateMutex(nullptr, true, "Unique string identifier_48161-XYZZY");

	// if another instance was found
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return true;
	}
	return false;
}