#pragma once

#include "shaderclass.h"

#define PLN_VS_RELPATH L"/shaders/v_plaintex.hlsl"
#define PLN_PS_RELPATH L"/shaders/p_plaintex.hlsl"

class PlainTextureShader: public Shader
{
public:
	struct MatrixBuffer
	{
		D3DXMATRIX finalMatrix;
		D3DXMATRIX worldMatrix;
		D3DXMATRIX rotMatrix;
		D3DXVECTOR4 camPosition;
	};

	PlainTextureShader();
	~PlainTextureShader();

	bool Initialize(ComPtr<ID3D11Device> const device);
	void Render(
		ComPtr<ID3D11DeviceContext> const,
		const MatrixBuffer&);

private:
	ComPtr<ID3D11Buffer> cnstMatrixBuffer;
};