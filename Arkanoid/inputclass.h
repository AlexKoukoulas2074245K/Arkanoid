#pragma once

#include <math.h>
#include  <map>

#include "common.h"

// Win 32 key code translation
#define VK_W 0x57
#define VK_S 0x53
#define VK_A 0x41
#define VK_D 0x44

// Custom key codes
#define N_KEYS  0x08

#define K_LEFT  0x00
#define K_RIGHT 0x01
#define K_UP    0x02
#define K_DOWN  0x03
#define K_W     0x04
#define K_S     0x05
#define K_A     0x06
#define K_D     0x07

class UserInput
{
public:
	struct Mouse
	{
		float x, y;
		float rx, ry;
		float dx, dy;
		bool lButton, rButton;
	};

	UserInput();
	~UserInput();

	void UpdateState(const MSG, const HWND);
	bool getKey(const UINT);
	Mouse getMouse() const;
	void ResetMouse(const HWND&);

private:
	bool keys[N_KEYS];
	std::map<UINT, UINT> keymappings;
	Mouse mouse;
};