#include "d3drendererclass.h"

D3DRenderer::D3DRenderer()
{

}

D3DRenderer::~D3DRenderer()
{
	LOG("Deleting D3DRenderer\n...");
}

ComPtr<ID3D11Device> D3DRenderer::getDevice() const
{
	return device;
}

ComPtr<ID3D11DeviceContext> D3DRenderer::getDeviceContext() const
{
	return devcon;
}

void D3DRenderer::BeginRendering(const bool includeDepth)
{
	if (includeDepth) devcon->OMSetRenderTargets(1, backBuffer.GetAddressOf(), depthStencilView.Get());
	else devcon->OMSetRenderTargets(1, backBuffer.GetAddressOf(), NULL);

	if (wireframe) devcon->RSSetState(wireframeRastState.Get());
	else devcon->RSSetState(defaultRastState.Get());
}

void D3DRenderer::ClearScreen(const D3DXVECTOR4& color, const bool includeDepth)
{
	float clearColor[] = {color.x, color.y, color.z, color.w};
	devcon->ClearRenderTargetView(backBuffer.Get(), clearColor);
	if (includeDepth) devcon->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DRenderer::SwapBuffers()
{
	swapchain->Present(1, 0);
}

void D3DRenderer::SwapRenderType()
{
	wireframe = !wireframe;
	BeginRendering(true);
}

bool D3DRenderer::Initialize(const HWND& hWindow)
{
	HRESULT result;

	wireframe = false;

	// Swap Chain Initialization
	DXGI_SWAP_CHAIN_DESC sChainDesc;
	ZeroMemory(&sChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	sChainDesc.BufferCount = 1;
	sChainDesc.OutputWindow = hWindow;
	sChainDesc.Windowed = false;
	sChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sChainDesc.BufferDesc.Width = GetSystemMetrics(SM_CXSCREEN);
	sChainDesc.BufferDesc.Height = GetSystemMetrics(SM_CYSCREEN);
	sChainDesc.BufferDesc.RefreshRate.Denominator = 0; // force
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

	// Back buffer description and initialization
	ID3D11Texture2D* bbTex;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &bbTex);

	result = device->CreateRenderTargetView(bbTex, NULL, &backBuffer);
	bbTex->Release();

	if (FAILED(result))
	{
		MessageBox(hWindow, "Back buffer creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	// Depth Buffer description and initialization
	D3D11_TEXTURE2D_DESC texd = {};
	texd.Width = WINDOW_WIDTH;
	texd.Height = WINDOW_HEIGHT;
	texd.ArraySize = 1;
	texd.MipLevels = 1;
	texd.SampleDesc.Count = 4;
	texd.Format = DXGI_FORMAT_D32_FLOAT;
	texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	result = device->CreateTexture2D(&texd, NULL, &depthBuffer);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Depth buffer texture creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	result = device->CreateDepthStencilView(depthBuffer.Get(), &dsvd, &depthStencilView);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Depth buffer creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	// Custom Blend State creation
	D3D11_BLEND_DESC blendDesc;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.AlphaToCoverageEnable = TRUE;

	result = device->CreateBlendState(&blendDesc, &blendState);
	if (FAILED(result))
	{
		MessageBox(NULL, "Blend state creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	devcon->OMSetBlendState(blendState.Get(), 0, 0xffffffff);

	// Viewport creation
	D3D11_VIEWPORT viewportDesc;
	ZeroMemory(&viewportDesc, sizeof(D3D11_VIEWPORT));

	viewportDesc.Width = WINDOW_WIDTH;
	viewportDesc.Height = WINDOW_HEIGHT;
	viewportDesc.TopLeftX = 0;
	viewportDesc.TopLeftY = 0;
	viewportDesc.MinDepth = 0;
	viewportDesc.MaxDepth = 1;

	devcon->RSSetViewports(1, &viewportDesc);

	D3D11_RASTERIZER_DESC rd = {};
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	rd.FrontCounterClockwise = FALSE;
	rd.DepthClipEnable = TRUE;
	rd.ScissorEnable = FALSE;
	rd.AntialiasedLineEnable = FALSE;
	rd.MultisampleEnable = FALSE;
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0.0f;
	rd.SlopeScaledDepthBias = 0.0f;

	result = device->CreateRasterizerState(&rd, &defaultRastState);

	if (FAILED(result))
	{
		MessageBox(NULL, "Default rasterizer state creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	rd.AntialiasedLineEnable = TRUE;
	rd.FillMode = D3D11_FILL_WIREFRAME;

	result = device->CreateRasterizerState(&rd, &wireframeRastState);

	if (FAILED(result))
	{
		MessageBox(NULL, "Wireframe rasterizer state creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	return true;
}
