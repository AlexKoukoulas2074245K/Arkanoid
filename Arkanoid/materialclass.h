#pragma once

// Custom Includes
#include "common.h"

class Material
{
public:
	Material();
	~Material();

	bool Initialize(ComPtr<ID3D11Device> const, const char* const);
	void Render(ComPtr<ID3D11DeviceContext> const);

private:
	// COM objects
	ComPtr<ID3D11ShaderResourceView> shaderResourceView;
};