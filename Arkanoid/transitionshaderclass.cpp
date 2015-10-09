#include "transitionshaderclass.h"

TransitionShader::TransitionShader()
{

}

TransitionShader::~TransitionShader()
{
	LOG("Deleting Transition Shader...\n");
}

bool TransitionShader::Initialize(ComPtr<ID3D11Device> const device)
{
	std::wstring vFilename = GetAssetPath(TRANS_VS_RELPATH);
	std::wstring pFilename = GetAssetPath(TRANS_PS_RELPATH);

	if (!Shader::Initialize(device, vFilename, pFilename))
	{
		return false;
	}

	HRESULT result;

	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.ByteWidth = 16;
	cbd.MiscFlags = 0;
	cbd.StructureByteStride = 0;
	cbd.CPUAccessFlags = 0;
	cbd.Usage = D3D11_USAGE_DEFAULT;

	result = device->CreateBuffer(&cbd, NULL, &cnstTransBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, "Transition constant buffer creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	return true;
}

void TransitionShader::Render(
	ComPtr<ID3D11DeviceContext> const devcon,
	const TransBuffer& transBuffer)
{
	Shader::Render(devcon);
	devcon->UpdateSubresource(cnstTransBuffer.Get(), NULL, NULL, &transBuffer, NULL, NULL);
	devcon->PSSetConstantBuffers(0, 1, cnstTransBuffer.GetAddressOf());
}