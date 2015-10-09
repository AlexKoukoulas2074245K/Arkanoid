#pragma once
#include "common.h"

class D3DRenderer
{

public:
	D3DRenderer();
	~D3DRenderer();
	bool Initialize(const HWND&);
	void BeginRendering(const bool);
	void ClearScreen(const D3DXVECTOR4&, const bool);
	void SwapBuffers();
	void SwapRenderType();

	ComPtr<ID3D11Device> getDevice() const;
	ComPtr<ID3D11DeviceContext> getDeviceContext() const;
	
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> devcon;
	ComPtr<ID3D11RenderTargetView> backBuffer;
	ComPtr<IDXGISwapChain> swapchain;
	ComPtr<ID3D11Texture2D> depthBuffer;
	ComPtr<ID3D11DepthStencilView> depthStencilView;
	ComPtr<ID3D11BlendState> blendState;
	ComPtr<ID3D11RasterizerState> defaultRastState;
	ComPtr<ID3D11RasterizerState> wireframeRastState;

	bool wireframe;
};