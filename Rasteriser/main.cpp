#include <Windows.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <chrono>


#include "App.h"
#include "InputManager.h"

const int WIDTH = 800;
const int HEIGHT = 400;

BITMAPINFO bitmap;
void* pixels = nullptr;
InputManager inputManager;

struct DebugMemoryData
{
	uint32_t allocations;
	uint32_t deallocations;

};

static DebugMemoryData debugMemoryData;

void* operator new(size_t size)
{
	debugMemoryData.allocations += size;
	return std::malloc(size);
}

void operator delete(void* address, size_t size)
{
	debugMemoryData.deallocations += size;
	free(address);
}

void OutputMemoryData()
{
	std::cout << "Allocated: " << debugMemoryData.allocations << "\nDeallocated: " << debugMemoryData.deallocations << "\nMemory Usage: " << debugMemoryData.allocations - debugMemoryData.deallocations << "\n";
}

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
	case WM_KEYDOWN:
	{
		inputManager.OnKeyDown(wParam);
		return 0;
	}
	case WM_KEYUP:
	{
		inputManager.OnKeyUp(wParam);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		inputManager.OnMouseMove((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		inputManager.OnLeftMouseButtonDown();
		return 0;
	}
	case WM_LBUTTONUP:
	{
		inputManager.OnLeftMouseButtonUp();
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
	srand(time(0));
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

	LARGE_INTEGER freq;
	LARGE_INTEGER previousTime;
	LARGE_INTEGER currentTime;
	double deltaTime;
	
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&previousTime);
	QueryPerformanceCounter(&currentTime);

	InvalidateRect(hwnd, nullptr, FALSE);

	App app(WIDTH, HEIGHT, &inputManager, (uint32_t*)pixels);

	MSG msg = {};
	bool isRunning = true;
	while (isRunning)
	{
		//auto start = std::chrono::high_resolution_clock::now();
		inputManager.Update();
		while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) { isRunning = false; }
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		QueryPerformanceCounter(&currentTime);

		deltaTime = static_cast<float>(currentTime.QuadPart - previousTime.QuadPart) / static_cast<float>(freq.QuadPart);
		previousTime = currentTime;
		//auto frameProcessStart = std::chrono::high_resolution_clock::now();
		app.ProcessFrame(hwnd, deltaTime);
		OutputMemoryData();
		/*auto end = std::chrono::high_resolution_clock::now();
		auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		auto processingDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - frameProcessStart);
		std::cout << "total processing time: " << processingDuration.count() << "ms" << std::endl;
		std::cout << "total time: " << totalDuration.count() << "ms" << std::endl;*/
		std::cout << "pause here" << std::endl;
	}
	return 0;

}