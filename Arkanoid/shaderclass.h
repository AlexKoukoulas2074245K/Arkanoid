#pragma once

// Includes
#include <map>
#include "common.h"
#include <d3dcompiler.h>

#pragma comment (lib, "d3dcompiler.lib")

// Constants
#define STDVSPROFILE "vs_5_0"
#define STDPSPROFILE "ps_5_0"
#define STDSHADERFUNC "main"
#define VSPREFIX L"v_"
#define PSPREFIX L"p_"

class Shader
{
public:
	virtual ~Shader();
	virtual bool Initialize(ComPtr<ID3D11Device> const, const std::wstring, const std::wstring);
	virtual void Render(ComPtr<ID3D11DeviceContext> const);
	virtual void* getShaderBCode() const;
	virtual SIZE_T getByteCodeLength() const;

protected:
	Shader();

protected:
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D10Blob> psBuffer;
	ComPtr<ID3D10Blob> vsBuffer;
};