#include <Windows.h>
#include <stdint.h>
#include <iostream>
#include <fstream>

#include "App.h"

const int WIDTH = 800;
const int HEIGHT = 400;

BITMAPINFO bitmap;
void* pixels = nullptr;

void OpenConsole() {
	if (AllocConsole()) {
		// Redirect std::cout to the console
		FILE* consoleOut;
		freopen_s(&consoleOut, "CONOUT$", "w", stdout);  // Use freopen_s for safer I/O

		// Optionally, redirect std::cin or std::cerr as needed
		FILE* consoleIn;
		freopen_s(&consoleIn, "CONIN$", "r", stdin);
		FILE* consoleErr;
		freopen_s(&consoleErr, "CONOUT$", "w", stderr);

		// You can use std::cout, std::cin, and std::cerr directly now
		//std::cout << "Console initialized successfully!" << std::endl;
	}
	else {
		std::cerr << "Failed to allocate console." << std::endl;
	}
}

//void DrawTestPattern()
//{
//	uint32_t* pixelData = (uint32_t*)pixels;
//	for (int y = 0; y < HEIGHT; ++y)
//	{
//		for (int x = 0; x < WIDTH; x++)
//		{
//			pixelData[(y * WIDTH) + x] = 0xFF0000;
//		}
//	}
//}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		StretchDIBits(
			hdc,
			0, 0, WIDTH, HEIGHT,
			0, 0, WIDTH, HEIGHT,
			pixels,
			&bitmap,
			DIB_RGB_COLORS,
			SRCCOPY
		);
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_ERASEBKGND:
	{
		return 1;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	OpenConsole();
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"PixelWindowClass";
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	RegisterClass(&wc);

	RECT windowRect = { 0, 0, WIDTH, HEIGHT };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winWidth = windowRect.right - windowRect.left;
	int winHeight = windowRect.bottom - windowRect.top;
	std::cout << winWidth << ", " << winHeight << "\n";
	HWND hwnd = CreateWindowEx(
		0,
		L"PixelWindowClass",
		L"Pixel Drawing Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, winWidth, winHeight,
		nullptr, nullptr, hInstance, nullptr
	);

	if (!hwnd)
	{
		MessageBox(nullptr, L"Faild to create a window", L"Error", MB_ICONERROR);
		return -1;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	ZeroMemory(&bitmap, sizeof(bitmap));
	bitmap.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmap.bmiHeader.biWidth = WIDTH;
	bitmap.bmiHeader.biHeight = -HEIGHT;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;

	HDC hdc = GetDC(hwnd);
	HBITMAP hBitmap = CreateDIBSection(hdc, &bitmap, DIB_RGB_COLORS, &pixels, nullptr, 0);
	ReleaseDC(hwnd, hdc);

	if (!pixels) {
		MessageBox(nullptr, L"Failed to allocate pixel buffer", L"Error", MB_ICONERROR);
		return -1;
	}

	//DrawTestPattern();

	InvalidateRect(hwnd, nullptr, FALSE);

	App app;
	app.InitApp(WIDTH, HEIGHT);

	MSG msg = {};
	bool isRunning = true;
	while (isRunning)
	{
		while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) { isRunning = false; }
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		app.ProcessFrame((uint32_t*)pixels, hwnd);
	}
	return 0;

}