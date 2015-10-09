#pragma once

#include "shaderclass.h"

#define DEF_VS_RELPATH L"/shaders/v_default.hlsl"
#define DEF_PS_RELPATH L"/shaders/p_default.hlsl"

// Light constants
#define L_AMBIENT_COLOR D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f)
#define L_DIFFUSE_COLOR D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)
#define L_LIGHT_DIRECTION D3DXVECTOR3(0.0f, 0.0f, -1.0f)
#define L_SPECULAR_POWER 256.0f
#define L_SPECULAR_COLOR D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)

class DefaultShader : public Shader
{
public:
	struct MatrixBuffer
	{
		D3DXMATRIX finalMatrix;
		D3DXMATRIX worldMatrix;
		D3DXMATRIX rotMatrix;
		D3DXVECTOR4 camPosition;
	};

	struct LightBuffer
	{
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float specularPower;
		D3DXVECTOR4 specularColor;
	};

	struct AlphaBuffer
	{
		bool dying;
		float alphaCounter;
	};

	DefaultShader();
	~DefaultShader();

	bool Initialize(ComPtr<ID3D11Device> const device);
	void Render(
		ComPtr<ID3D11DeviceContext> const,
		const MatrixBuffer&,
		const LightBuffer&,
		const AlphaBuffer&);

private:
	ComPtr<ID3D11Buffer> cnstMatrixBuffer;
	ComPtr<ID3D11Buffer> cnstLightBuffer;
	ComPtr<ID3D11Buffer> cnstAlphaBuffer;
};