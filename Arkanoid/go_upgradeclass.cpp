#include "go_upgradeclass.h"

char* GO_Upgrade::materials[N_UPTYPES] = {RED_MAT, GREEN_MAT, BLUE_MAT};

GO_Upgrade::GO_Upgrade()
{

}

GO_Upgrade::~GO_Upgrade()
{
	LOG("Deleting GO_Upgrade...\n");
}

bool GO_Upgrade::Initialize(
	ComPtr<ID3D11Device> const device,
	const D3DXVECTOR3 position,
	const std::shared_ptr<ShaderManager> shaderManager,
	const std::shared_ptr<MaterialFactory> matFactory)
{
	GameObject::Initialize(device, shaderManager, matFactory);

	alive = true;
	upgrFlag = BD_FLAG_NONE;
	upgrType = std::rand() % N_UPTYPES;

	if (upgrType == UPT_RED) upgrFlag = BD_FLAG_RED;
	else if (upgrType == UPT_GREEN) upgrFlag = BD_FLAG_GREEN;
	else if (upgrType == UPT_BLUE) upgrFlag = BD_FLAG_BLUE;

	Model::ModelType modelDesc;
	modelDesc.modelFilename = "/models/upgrade.obj";
	modelDesc.materialFilename = GO_Upgrade::materials[upgrType];

	if (!model.Initialize(
		modelDesc,
		device,
		shaderManager->getDefaultShader()->getShaderBCode(),
		shaderManager->getDefaultShader()->getByteCodeLength(),
		matFactory))
	{
		return false;
	}

	setTranslation(position);
	ScaleBy(-0.4f);

	return true;
}

void GO_Upgrade::Update()
{
	RotateBy(0.0f, 0.0087f, 0.0f);
	TranslateBy(0.0f, -0.1f, 0.0f);
	if (transform.translation.y < UPGR_KILL_Y) alive = false;

	GameObject::Update();
}

void GO_Upgrade::Render(
	ComPtr<ID3D11DeviceContext> const devcon,
	const D3DXMATRIX& viewProjection,
	const D3DXVECTOR3& camPos)
{
	GameObject::Render(devcon, viewProjection, camPos);

	Model::MatrixBuffer mb = model.getMatrices();

	DefaultShader::MatrixBuffer shaderMtrx;
	shaderMtrx.worldMatrix = mb.worldMatrix;
	shaderMtrx.finalMatrix = mb.finalMatrix;
	shaderMtrx.rotMatrix = mb.rotMatrix;
	shaderMtrx.camPosition = D3DXVECTOR4(camPos.x, camPos.y, camPos.z, 0.0f);

	DefaultShader::LightBuffer lightBuffer;
	lightBuffer.ambientColor = L_AMBIENT_COLOR;
	lightBuffer.diffuseColor = L_DIFFUSE_COLOR;
	lightBuffer.lightDirection = L_LIGHT_DIRECTION;
	lightBuffer.specularPower = L_SPECULAR_POWER;
	lightBuffer.specularColor = L_SPECULAR_COLOR;

	DefaultShader::AlphaBuffer alphaBuffer;
	alphaBuffer.dying = false;
	alphaBuffer.alphaCounter = 1.0f;

	shaderManager->getDefaultShader()->Render(devcon, shaderMtrx, lightBuffer, alphaBuffer);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(model.getIndexCount(), 0, 0);
}

void GO_Upgrade::Hit()
{
	alive = false;
}

bool GO_Upgrade::getAlive() const
{
	return alive;
}

upgr_type GO_Upgrade::getType() const
{
	return upgrType;
}

upgr_flag GO_Upgrade::getFlag() const
{
	return upgrFlag;
}