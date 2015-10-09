#include "go_boardclass.h"

GO_Board::GO_Board()
{

}

GO_Board::~GO_Board()
{

}

bool GO_Board::Initialize(
	ComPtr<ID3D11Device> const device,
	const std::shared_ptr<ShaderManager> shaderManager,
	const std::shared_ptr<MaterialFactory> materialFactory)
{
	GameObject::Initialize(device, shaderManager, materialFactory);

	Model::ModelType modelDesc;
	modelDesc.modelFilename = "/models/board.obj";
	modelDesc.materialFilename = "/textures/board.dds";

	if (!model.Initialize(
		modelDesc,
		device,
		shaderManager->getDefaultShader()->getShaderBCode(),
		shaderManager->getDefaultShader()->getByteCodeLength(),
		materialFactory))
	{
		MessageBox(NULL, "Board model creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}
	
	setScale(BOARD_INIT_SCALE.x);
	setTranslation(BOARD_INIT_TRANSL);

	return true;
}

void GO_Board::Move(const float mouseDX)
{
	TranslateBy(mouseDX / MOUSE_DELTA_TOL, 0.0f, 0.0f);
	if (transform.translation.x > LVL_LEFT_BOUNDARY) transform.translation.x = LVL_LEFT_BOUNDARY;
	if (transform.translation.x < LVL_RIGHT_BOUNDARY) transform.translation.x = LVL_RIGHT_BOUNDARY;
}

void GO_Board::Update()
{
	GameObject::Update();
}

void GO_Board::Render(
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