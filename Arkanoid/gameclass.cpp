#include "gameclass.h"

Game::Game(const HWND hwnd)
{
	this->hWindow = hwnd;
}

bool Game::Initialize()
{
	if (!InitD3D())
	{
		return false;
	}

	if (!InitComponents())
	{
		return false;
	}

	InitExtras();

	return true;
}

Game::~Game()
{

}

void Game::Update()
{
	fpsCounter.Update();
	cpuCounter.Update();

	temp += 0.05f;

	std::string stats = "Fps: " + std::to_string(fpsCounter.GetFps()) +
					    " - Cpu Usage: " + std::to_string(cpuCounter.GetCpuPercentage()) + "%";
	SetWindowText(hWindow, stats.c_str());
	SetMatrices();
}

void Game::Render()
{
	// devcon->OMSetDepthStencilState(depthStencilState, 1);
	devcon->OMSetRenderTargets(1, &backBuffer, NULL);
	
	ClearScreen();
	model.Render(devcon, finalMatrix);
	swapchain->Present(1, 0);
}

bool Game::InitD3D()
{
	HRESULT result;

	DXGI_SWAP_CHAIN_DESC sChainDesc;
	ZeroMemory(&sChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	sChainDesc.BufferCount = 1;
	sChainDesc.OutputWindow = hWindow;
	sChainDesc.Windowed = WINDOWED;
	sChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sChainDesc.BufferDesc.Width = WINDOW_WIDTH;
	sChainDesc.BufferDesc.Height = WINDOW_HEIGHT;
	sChainDesc.BufferDesc.RefreshRate.Denominator = 60; // force
	sChainDesc.BufferDesc.RefreshRate.Numerator = 1; // v-sync
	sChainDesc.SampleDesc.Count = 4;


	result = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&sChainDesc,
		&swapchain,
		&device,
		NULL,
		&devcon);

	if (FAILED(result))
	{
		MessageBox(hWindow, "Device and Swapchain creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	ID3D11Texture2D *bbTex;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &bbTex);

	result = device->CreateRenderTargetView(bbTex, NULL, &backBuffer);
	bbTex->Release();
	
	if (FAILED(result))
	{
		MessageBox(hWindow, "Back buffer creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}


	// create the depth buffer texture
	D3D11_TEXTURE2D_DESC texd;
	ZeroMemory(&texd, sizeof(texd));

	texd.Width = WINDOW_WIDTH;
	texd.Height = WINDOW_HEIGHT;
	texd.ArraySize = 1;
	texd.MipLevels = 1;
	texd.SampleDesc.Count = 1;
	texd.Format = DXGI_FORMAT_D32_FLOAT;
	texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D *pDepthBuffer;
	result = device->CreateTexture2D(&texd, NULL, &pDepthBuffer);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Depth buffer creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	// Depth stencild desc
	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = true;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;

	dsd.StencilEnable = true;
	dsd.StencilReadMask = 0xFF;
	dsd.StencilWriteMask = 0xFF;

	dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = device->CreateDepthStencilState(&dsd, &depthStencilState);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Custom depth stencil state creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	// create the depth buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));

	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	device->CreateDepthStencilView(pDepthBuffer, &dsvd, &depthStencilView);

	return true;
}

bool Game::InitComponents()
{
	Model::ModelType modelDesc = {};
	modelDesc.typeFlag = PRIMITIVE;

	if (!model.Initialize(modelDesc, device))
	{
		return false;
	}


	D3D11_VIEWPORT viewportDesc;
	ZeroMemory(&viewportDesc, sizeof(D3D11_VIEWPORT));

	viewportDesc.Width = WINDOW_WIDTH;
	viewportDesc.Height = WINDOW_HEIGHT;
	viewportDesc.TopLeftX = 0;
	viewportDesc.TopLeftY = 0;
	viewportDesc.MinDepth = 0;
	viewportDesc.MaxDepth = 1;

	devcon->RSSetViewports(1, &viewportDesc);

	return true;
}

void Game::InitExtras()
{
	cpuCounter.Initialize();
	fpsCounter.Initialize();
	temp = 0.0f;
}

void Game::ClearScreen()
{
	float clearColor[] = {0.2f, 0.4f, 0.8f, 1.0f};
	devcon->ClearRenderTargetView(backBuffer, clearColor);
}

void Game::SetMatrices()
{
	XMMATRIX transMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMMATRIX rotMatrixY = XMMatrixRotationY(temp);
	XMMATRIX scaleMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	worldMatrix = rotMatrixY * scaleMatrix * transMatrix;

	XMVECTOR cameraPos = XMVectorSet(1.5f, 0.5f, 4.5f, 0.0f);
	XMVECTOR targetPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	viewMatrix = XMMatrixLookAtLH(cameraPos, targetPos, upVector);

	float frustumAngle = XMConvertToRadians(45);
	float zNear = 0.1f;
	float zFar = 100.0f;
	float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	projectionMatrix = XMMatrixPerspectiveFovLH(frustumAngle, aspectRatio, zNear, zFar);

	finalMatrix = worldMatrix * viewMatrix * projectionMatrix;
	finalMatrix = XMMatrixTranspose(finalMatrix);
}

void Game::Shutdown()
{
	if (device) device->Release();
	if (devcon) devcon->Release();
	if (swapchain) swapchain->Release();
	if (backBuffer) backBuffer->Release();
	if (depthStencilBuffer) depthStencilBuffer->Release();
	if (depthStencilState) depthStencilState->Release();
	if (depthStencilView) depthStencilView->Release();
	model.Shutdown();
	cpuCounter.Shutdown();
}