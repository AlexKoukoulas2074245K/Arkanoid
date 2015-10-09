#include "plaintexshaderclass.h"

PlainTextureShader::PlainTextureShader()
{

}

PlainTextureShader::~PlainTextureShader()
{
	LOG("Deleting Default Shader...\n");
}

void PlainTextureShader::Render(
	ComPtr<ID3D11DeviceContext> const devcon,
	const MatrixBuffer& matrixBuffer)
{
	Shader::Render(devcon);
	devcon->UpdateSubresource(cnstMatrixBuffer.Get(), NULL, NULL, &matrixBuffer, NULL, NULL);
	devcon->VSSetConstantBuffers(0, 1, cnstMatrixBuffer.GetAddressOf()); // Always after updated subresources
}


bool PlainTextureShader::Initialize(ComPtr<ID3D11Device> const dev)
{
	std::wstring vFilename = GetAssetPath(PLN_VS_RELPATH);
	std::wstring pFilename = GetAssetPath(PLN_PS_RELPATH);

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

	return true;
}