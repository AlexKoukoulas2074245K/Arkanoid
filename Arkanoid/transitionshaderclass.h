#pragma once

#include "shaderclass.h"

#define TRANS_VS_RELPATH L"/shaders/v_transition.hlsl"
#define TRANS_PS_RELPATH L"/shaders/p_transition.hlsl"

class TransitionShader: public Shader
{
public:
	struct TransBuffer
	{
		float alphaCounter;
	};

	TransitionShader();
	~TransitionShader();

	bool Initialize(ComPtr<ID3D11Device> const device);
	void Render(
		ComPtr<ID3D11DeviceContext> const,
		const TransBuffer&);

private:
	ComPtr<ID3D11Buffer> cnstTransBuffer;
};