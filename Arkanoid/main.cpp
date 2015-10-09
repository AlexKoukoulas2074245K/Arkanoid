#include "main.h"

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWindow;
	WNDCLASSEX wndDesc;

	ZeroMemory(&wndDesc, sizeof(WNDCLASSEX));
	
	wndDesc.cbSize = sizeof(WNDCLASSEX);
	wndDesc.style = CS_HREDRAW | CS_VREDRAW;
	wndDesc.lpfnWndProc = WindowProc;
	wndDesc.hInstance = hInstance;
	wndDesc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndDesc.hbrBackground = (HBRUSH) COLOR_WINDOW;
	wndDesc.lpszClassName = "WindowClass1";

	RegisterClassEx(&wndDesc);

	unsigned int monitorWidth = GetSystemMetrics(SM_CXSCREEN);
	unsigned int monitorHeight = GetSystemMetrics(SM_CYSCREEN);
	
	DEVMODE dmScreenSettings;
	// If full screen set the screen to maximum size of the users desktop and 32bit.
	memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
	dmScreenSettings.dmSize = sizeof(dmScreenSettings);
	dmScreenSettings.dmPelsWidth = (unsigned long) WINDOW_WIDTH;
	dmScreenSettings.dmPelsHeight = (unsigned long) WINDOW_HEIGHT;
	dmScreenSettings.dmBitsPerPel = 32;
	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	// Change the display settings to full screen.
	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

	hWindow = CreateWindowEx(
		NULL,
		"WindowClass1",
		"MyWindow",
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		0,
		0,
		monitorWidth,
		monitorHeight,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWindow, nCmdShow);
	ShowCursor(false);

	bool running = true;
	MSG message;
	
	GameStateManager gsm;

	if (!gsm.Initialize(hWindow))
	{
		ShowCursor(true);
		return 1;
	}

	while (running)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
			
			if (message.message == WM_QUIT)
			{
				running = false;
			}
			else
			{
				gsm.HandleInput(message);
			}
		}
		else
		{
			gsm.Update();
			
			if (!gsm.isFinished())
			{
				gsm.Render();
			}
			else
			{
				running = false;
			}
		}
	}		

	ShowCursor(true);
	return 0;
}

LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;
		
		case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE)
			{
				PostQuitMessage(0);
				return 0;
			}
		}break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
