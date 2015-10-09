#include "inputclass.h"

UserInput::UserInput()
{
	for (int i = 0; i < N_KEYS; i++)
	{
		keys[i] = false;
	}
	keymappings.insert(std::pair<UINT, UINT>(VK_LEFT, K_LEFT));
	keymappings.insert(std::pair<UINT, UINT>(VK_RIGHT, K_RIGHT));
	keymappings.insert(std::pair<UINT, UINT>(VK_UP, K_UP));
	keymappings.insert(std::pair<UINT, UINT>(VK_DOWN, K_DOWN));
	keymappings.insert(std::pair<UINT, UINT>(VK_W, K_W));
	keymappings.insert(std::pair<UINT, UINT>(VK_S, K_S));
	keymappings.insert(std::pair<UINT, UINT>(VK_A, K_A));
	keymappings.insert(std::pair<UINT, UINT>(VK_D, K_D));

	mouse = {};
}

UserInput::~UserInput()
{

}

bool UserInput::getKey(UINT key)
{
	return keys[key];
}

UserInput::Mouse UserInput::getMouse() const
{
	return mouse;
}

void UserInput::ResetMouse(const HWND& hwnd)
{
	POINT reset;
	reset.x = static_cast<LONG>(WINDOW_WIDTH / 2.0f);
	reset.y = static_cast<LONG>(WINDOW_HEIGHT / 2.0f);
	mouse.rx = static_cast<float>(reset.x);
	mouse.ry = static_cast<float>(reset.y);

	ClientToScreen(hwnd, &reset);
	SetCursorPos(reset.x, reset.y);
}

void UserInput::UpdateState(MSG msg, HWND hwnd)
{
	if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
	{
		if (keymappings.count(msg.wParam))
		{
			keys[keymappings.at(msg.wParam)] = msg.message == WM_KEYDOWN;
		}
	}
	else if (msg.message == WM_MOUSEMOVE)
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(hwnd, &p);
		mouse.x = static_cast<float>(p.x);
		mouse.y = static_cast<float>(p.y);
		mouse.dx = mouse.rx - mouse.x;
	}
	else if (msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONUP)
	{
		mouse.lButton = msg.message == WM_LBUTTONDOWN;
	}
	else if (msg.message == WM_RBUTTONDOWN || msg.message == WM_RBUTTONUP)
	{
		mouse.rButton = msg.message == WM_RBUTTONDOWN;
	}
}