#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <dxerr.h>
#include <iostream>
#include <string>
#include <memory>
#include <wrl\client.h>

// Linking
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "dxerr.lib")

using Microsoft::WRL::ComPtr;

// Debug Switch
#define DEBUG

// Math
#define MATH_PI static_cast<float>(D3DX_PI)

// Relabels
#define INDEX_FORMAT DXGI_FORMAT_R32_UINT
#define POS_FORMAT DXGI_FORMAT_R32G32B32_FLOAT
#define TEXCOORD_FORMAT DXGI_FORMAT_R32G32_FLOAT
#define NORMAL_FORMAT DXGI_FORMAT_R32G32B32_FLOAT

// Level Edges
#define LVL_LEFT_BOUNDARY 10.0f
#define LVL_RIGHT_BOUNDARY -10.0f
#define LVL_UP_BOUNDARY 9.0f
#define LVL_DOWN_BOUNDARY -11.0f

// Typedefs
typedef unsigned int indexType;

// Rendering
struct Vertex
{
	float x, y, z;
	float u, v;
	float nx, ny, nz;
};

// Cartesian
enum AXIS
{
	X, Y, Z
};

// Window
#define WINDOW_WIDTH GetSystemMetrics(SM_CXSCREEN)
#define WINDOW_HEIGHT GetSystemMetrics(SM_CYSCREEN)

// Logging
#if defined(DEBUG)
#define LOG(x) OutputDebugString(x); 
#else
#define LOG(x)
#endif

// Constants
#define WINDOWED TRUE
#define MSAA 1

// Custom Message Box titles
#define FATALERROR "Fatal Error!"
#define WARNING "Waring!"

// Custom Message Layout
#define FATALLAYOUT MB_ICONERROR | MB_OK
#define WARNLAYOUT MB_ICONWARNING | MB_OK

// Asset paths
#define ASSETROOT "assets"
#define W_ASSETROOT L"assets"

// Inlines
inline std::string GetAssetPath(const char* const relative)
{
	return ASSETROOT + std::string(relative);
}

inline std::wstring GetAssetPath(const wchar_t* const relative)
{
	return W_ASSETROOT + std::wstring(relative);
}

inline std::string GetAssetPath(const std::string relative)
{
	return ASSETROOT + relative;
}