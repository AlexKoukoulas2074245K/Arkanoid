#include "go_blockclass.h"

GO_Block::GO_Block()
{

}

GO_Block::~GO_Block()
{
	LOG("Deleting GO_Block...\n");
}

bool GO_Block::Initialize(
	ComPtr<ID3D11Device> const device,
	const float blockX,
	const float blockY,
	const int blockType,
	const std::shared_ptr<ShaderManager> shaderManager,
	const std::shared_ptr<MaterialFactory> materialFactory)
{
	GameObject::Initialize(device, shaderManager, materialFactory);

	Model::ModelType modelDesc = {};
	std::string matfname = "/textures/blocks/block";
	matfname += std::to_string(blockType);
	matfname += ".dds";
	modelDesc.materialFilename = matfname.c_str();
	modelDesc.modelFilename = "/models/block.obj";
	
	if (!model.Initialize(
		modelDesc,
		device,
		shaderManager->getDefaultShader()->getShaderBCode(),
		shaderManager->getDefaultShader()->getByteCodeLength(),
		materialFactory))
	{
		MessageBox(NULL, "Block model creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	ScaleBy(-0.5f);
	setTranslation(blockX, blockY, 0.0f);
	alphaCounter = 1.0f;
	hit = false;
	alive = true;
	aniFinished = false;
	invisible = blockType == 5;

	return true;
}

void GO_Block::Update()
{
	GameObject::Update();

	if (!hit) return;
	
	if (alphaCounter > 0.0f)
	{
		alphaCounter -= 0.05f;
	}
	else
	{
		alphaCounter = 0.0f;
		alive = false;
	}
}

void GO_Block::Render(
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
	alphaBuffer.dying = hit;
	alphaBuffer.alphaCounter = alphaCounter;

	shaderManager->getDefaultShader()->Render(devcon, shaderMtrx, lightBuffer, alphaBuffer);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(model.getIndexCount(), 0, 0);
}

void GO_Block::Animate(const float angle)
{
	if (hit) return;

	aniFinished = false;
	
	RotateBy(BLK_RAD_ROT, 0.0f, 0.0f);
	if (getRotation().x >= static_cast<float>(D3DXToRadian(angle)))
	{
		RotateBy(static_cast<float>(-D3DXToRadian(angle)), 0.0f, 0.0f);
		aniFinished = true;
	}
}

void GO_Block::Hit()
{
	if (invisible) invisible = false;
	else hit = true;
}

bool GO_Block::getAlive() const
{
	return alive;
}

bool GO_Block::getHit() const
{
	return hit;
}

bool GO_Block::getInvisible() const
{
	return invisible;
}

bool GO_Block::getAniFinish() const
{
	return aniFinished;
}