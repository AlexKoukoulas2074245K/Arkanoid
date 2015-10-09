#include "materialclass.h"

Material::Material()
{

}

Material::~Material()
{
	LOG("Deleting Material...\n");
}

bool Material::Initialize(ComPtr<ID3D11Device> const device, const char* const filename)
{
	HRESULT result;
	
	result = D3DX11CreateShaderResourceViewFromFile(
		device.Get(),
		GetAssetPath(filename).c_str(),
		NULL,
		NULL,
		&shaderResourceView,
		NULL);

	if (FAILED(result))
	{
		MessageBox(NULL, "Shader resource view creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	return true;
}

void Material::Render(ComPtr<ID3D11DeviceContext> const devcon)
{
	devcon->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());
}