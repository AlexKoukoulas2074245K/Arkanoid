#pragma once

#include <string>

// Custom includes
#include "main.h"
#include "common.h"
#include "modelclass.h"
#include "rasterfpsclass.h"
#include "rastercpuclass.h"



class Game
{
public:
	Game(const HWND);
	~Game();
	bool Initialize();
	void Update();
	void Render();
	void Shutdown();

private:
	bool InitD3D();
	bool InitComponents();
	void InitExtras();
	void SetMatrices();
	inline void ClearScreen();

private:
	HWND hWindow;

	// COM objects
	ID3D11Device* device;
	ID3D11DeviceContext* devcon;
	ID3D11RenderTargetView* backBuffer;
	IDXGISwapChain* swapchain;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;

	// Math
	XMMATRIX worldMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	XMMATRIX finalMatrix;

	// Regular
	Model model;
	FpsClass fpsCounter;
	CpuClass cpuCounter;
	float temp;
};
