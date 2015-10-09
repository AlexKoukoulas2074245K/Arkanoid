#include "defaultshaderclass.h"

DefaultShader::DefaultShader()
{
	
}

DefaultShader::~DefaultShader()
{
	LOG("Deleting Default Shader...\n");
}

void DefaultShader::Render(
	ComPtr<ID3D11DeviceContext> const devcon,
	const MatrixBuffer& matrixBuffer,
	const LightBuffer& lightBuffer,
	const AlphaBuffer& alphaBuffer)
{
	Shader::Render(devcon);
	devcon->UpdateSubresource(cnstMatrixBuffer.Get(), NULL, NULL, &matrixBuffer, NULL, NULL);
	devcon->UpdateSubresource(cnstLightBuffer.Get(), NULL, NULL, &lightBuffer, NULL, NULL);
	devcon->UpdateSubresource(cnstAlphaBuffer.Get(), NULL, NULL, &alphaBuffer, NULL, NULL);
	devcon->VSSetConstantBuffers(0, 1, cnstMatrixBuffer.GetAddressOf()); // Always after updated subresources
	devcon->PSSetConstantBuffers(0, 1, cnstLightBuffer.GetAddressOf());
	devcon->PSSetConstantBuffers(1, 1, cnstAlphaBuffer.GetAddressOf());
}


bool DefaultShader::Initialize(ComPtr<ID3D11Device> const dev)
{
	std::wstring vFilename = GetAssetPath(DEF_VS_RELPATH);
	std::wstring pFilename = GetAssetPath(DEF_PS_RELPATH);
	
	if (!Shader::Initialize(dev, vFilename, pFilename))
	{
		return false;
	}

	HRESULT result;
	
	
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.ByteWidth = sizeof(MatrixBuffer);
	cbd.MiscFlags = 0;
	cbd.StructureByteStride = 0;
	cbd.CPUAccessFlags = 0;
	cbd.Usage = D3D11_USAGE_DEFAULT;

	result = dev->CreateBuffer(&cbd, NULL, &cnstMatrixBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, "Matrix constant buffer creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	cbd.ByteWidth = sizeof(LightBuffer);

	result = dev->CreateBuffer(&cbd, NULL, &cnstLightBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, "Light constant buffer creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	cbd.ByteWidth = 16;
	
	result = dev->CreateBuffer(&cbd, NULL, &cnstAlphaBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, "Light constant buffer creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	return true;
}