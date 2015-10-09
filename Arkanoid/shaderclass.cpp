#include "shaderclass.h"

Shader::Shader()
{

}

Shader::~Shader()
{
	LOG("Deleting Shader...\n");
}

bool Shader::Initialize(
	ComPtr<ID3D11Device> const dev,
	const std::wstring vFilename, 
	const std::wstring pFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;

	result = D3DCompileFromFile(
		vFilename.c_str(),
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		STDSHADERFUNC,
		STDVSPROFILE,
		D3D10_SHADER_ENABLE_STRICTNESS,
		NULL,
		&vsBuffer,
		&errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			std::wstring compileError = L"Shader compile error: " + vFilename;
			char* errorDesc = static_cast<char*>(errorMessage->GetBufferPointer());
			LOG("\n");
			LOG(errorDesc);
			LOG("\n");
			MessageBoxW(NULL, compileError.c_str(), L"Fatal Error", FATALLAYOUT);
		}
		else
		{
			std::wstring missingFile = L"Missing shader file: " + vFilename;
			MessageBoxW(NULL, missingFile.c_str(), L"Fatal Error", FATALLAYOUT);
		}

		return false;
	}

	result = dev->CreateVertexShader(
		vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(),
		NULL,
		&vertexShader);

	if (FAILED(result))
	{
		MessageBox(NULL, "Vertex Shader creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	result = D3DCompileFromFile(
		pFilename.c_str(),
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		STDSHADERFUNC,
		STDPSPROFILE,
		D3D10_SHADER_ENABLE_STRICTNESS,
		NULL,
		&psBuffer,
		&errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			std::wstring compileError = L"Shader compile error: " + pFilename;
			char* errorDesc = static_cast<char*>(errorMessage->GetBufferPointer());
			LOG("\n");
			LOG(errorDesc);
			LOG("\n");
			MessageBoxW(NULL, compileError.c_str(), L"Fatal Error", FATALLAYOUT);
		}
		else
		{
			std::wstring missingFile = L"Missing shader file: " + pFilename;
			MessageBoxW(NULL, missingFile.c_str(), L"Fatal Error", FATALLAYOUT);
		}

		return false;
	}

	result = dev->CreatePixelShader(
		psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(),
		NULL,
		&pixelShader);

	if (FAILED(result))
	{
		MessageBox(NULL, "Vertex Shader creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	if (errorMessage) errorMessage->Release();

	return true;
}

void Shader::Render(ComPtr<ID3D11DeviceContext> const devcon)
{
	devcon->VSSetShader(vertexShader.Get(), NULL, NULL);
	devcon->PSSetShader(pixelShader.Get(), NULL, NULL);
}

void* Shader::getShaderBCode() const
{
	return vsBuffer->GetBufferPointer();
}

SIZE_T Shader::getByteCodeLength() const
{
	return vsBuffer->GetBufferSize();
}