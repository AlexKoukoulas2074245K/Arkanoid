#include "go_backgroundclass.h"

GO_Background::GO_Background()
{

}

GO_Background::~GO_Background()
{

}

bool GO_Background::Initialize(
	ComPtr<ID3D11Device> const device,
	const std::shared_ptr<ShaderManager> shaderManager,
	const std::shared_ptr<MaterialFactory> matFactory)
{
	GameObject::Initialize(device, shaderManager, matFactory);

	Model::ModelType bgDesc;
	bgDesc.materialFilename = BG_MATERIAL;
	bgDesc.modelFilename = BG_MODEL;

	if (!model.Initialize(
		bgDesc,
		device,
		shaderManager->getDefaultShader()->getShaderBCode(),
		shaderManager->getPlainTextureShader()->getByteCodeLength(),
		matFactory))
	{
		return false;
	}
	
	TranslateBy(0.0f, 0.0f, -8.0f);
	ScaleBy(0.6f);

	return true;
}

void GO_Background::Update()
{
	GameObject::Update();
}

void GO_Background::Render(
	ComPtr<ID3D11DeviceContext> const devcon,
	const D3DXMATRIX& viewProjection,
	const D3DXVECTOR3& camPos)
{
	GameObject::Render(devcon, viewProjection, camPos);

	Model::MatrixBuffer mb = model.getMatrices();

	PlainTextureShader::MatrixBuffer shaderMtrx;
	shaderMtrx.worldMatrix = mb.worldMatrix;
	shaderMtrx.finalMatrix = mb.finalMatrix;
	shaderMtrx.rotMatrix = mb.rotMatrix;
	shaderMtrx.camPosition = D3DXVECTOR4(camPos.x, camPos.y, camPos.z, 0.0f);

	shaderManager->getPlainTextureShader()->Render(devcon, shaderMtrx);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(model.getIndexCount(), 0, 0);
}
