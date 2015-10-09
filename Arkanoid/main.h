#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include "gamestatemanagerclass.h"
#include "vld.h"

LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);