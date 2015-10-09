#include "go_ballclass.h"

std::map<unsigned int, char*> GO_Ball::materials(
{
	{BD_FLAG_NONE, "/textures/ball/ballbase.dds"},
	{BD_FLAG_RED, "/textures/ball/ballred.dds"},
	{BD_FLAG_BLUE, "/textures/ball/ballblue.dds"},
	{BD_FLAG_GREEN, "/textures/ball/ballgreen.dds"},
	{BD_FLAG_RED | BD_FLAG_GREEN, "/textures/ball/ballyellow.dds"},
	{BD_FLAG_RED | BD_FLAG_BLUE, "/textures/ball/ballpink.dds"},
	{BD_FLAG_BLUE | BD_FLAG_GREEN, "/textures/ball/ballcyan.dds"},
});

GO_Ball::GO_Ball()
{
	
}

GO_Ball::~GO_Ball()
{
	LOG("Deleting GO_BALL...\n");
}

bool GO_Ball::Initialize(
	ComPtr<ID3D11Device> const device,
	const std::shared_ptr<ShaderManager> shaderManager,
	const std::shared_ptr<MaterialFactory> materialFactory)
{
	GameObject::Initialize(device, shaderManager, materialFactory);
	currbody = BD_FLAG_NONE;

	Model::ModelType ballType = {};
	ballType.modelFilename = "/models/ball.obj";
	
	ballType.materialFilename = materials[currbody];

	if (!model.Initialize(
		ballType,
		device,
		shaderManager->getDefaultShader()->getShaderBCode(),
		shaderManager->getDefaultShader()->getByteCodeLength(),
		materialFactory))
	{
		MessageBox(NULL, "Ball model creation failed", FATALERROR, FATALLAYOUT);
		return false;
	}

	vel = BALL_INIT_VEL;
	transform.translation = BALL_INIT_TRANSL;
	unchained = false;
	alive = true;
	bounces = 0;

	return true;
}

D3DXVECTOR3 GO_Ball::getVel() const
{
	return vel;
}

void GO_Ball::setVelX(const float velX)
{
	vel.x = velX;
}

void GO_Ball::ReverseVel(const bool hor, const bool paddle)
{
	if (hor) vel.x *= -1;
	else vel.y *= -1;

	if (bounces <= MAX_BOUNCES)
	{
		vel *= BALL_CONST_ACCEL;
		bounces++;
	}
}

void GO_Ball::Update()
{
	if (unchained) transform.translation += vel;

	if (transform.translation.x >= LVL_LEFT_BOUNDARY + 2.0f)
	{
		transform.translation.x = LVL_LEFT_BOUNDARY + 2.0f;
		ReverseVel(true, false);
	}
	if (transform.translation.x <= LVL_RIGHT_BOUNDARY - 2.0f)
	{
		transform.translation.x = LVL_RIGHT_BOUNDARY - 2.0f;
		ReverseVel(true, false);
	}
	if (transform.translation.y >= LVL_UP_BOUNDARY)
	{
		transform.translation.y = LVL_UP_BOUNDARY;
		ReverseVel(false, false);
	}
	if (transform.translation.y <= LVL_DOWN_BOUNDARY)
	{
		alive = false;
	}

	GameObject::Update();
}

void GO_Ball::Upgrade(
	const upgr_flag upgradeFlag,
	ComPtr<ID3D11Device> const device,
	const std::shared_ptr<MaterialFactory> matFactory)
{
	switch (upgradeFlag)
	{
		case BD_FLAG_RED:
		{
			setOnFire();
		}break;

		case BD_FLAG_GREEN:
		{
			setOnGrass();
		}break;

		case BD_FLAG_BLUE:
		{
			setOnWater();
		}break;
	}

	model.newMaterial(materials[currbody], device, matFactory);
}

void GO_Ball::Render(
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

void GO_Ball::ResetVel()
{
	unchained = false;
	vel = BALL_INIT_VEL;
}

void GO_Ball::ResetBounces()
{
	bounces = 0;
}

bool GO_Ball::getUnchained() const
{
	return unchained;
}

bool GO_Ball::getOnFire() const
{
	return (currbody & BD_FLAG_RED) != 0;
}

bool GO_Ball::getOnGrass() const
{
	return (currbody & BD_FLAG_GREEN) != 0;
}

bool GO_Ball::getOnWater() const
{
	return (currbody & BD_FLAG_BLUE) != 0;
}

bool GO_Ball::getAlive() const
{
	return alive;
}

void GO_Ball::setUnchained(const bool unchained)
{
	this->unchained = unchained;
	this->vel = BALL_INIT_VEL;
}

void GO_Ball::setOnFire()
{
	if ((currbody & BD_FLAG_BLUE) && (currbody & BD_FLAG_GREEN)) currbody = BD_FLAG_RED;
	else currbody |= BD_FLAG_RED;
}

void GO_Ball::setOnGrass()
{
	if ((currbody & BD_FLAG_RED) && (currbody & BD_FLAG_BLUE)) currbody = BD_FLAG_GREEN;
	else currbody |= BD_FLAG_GREEN;
}

void GO_Ball::setOnWater()
{
	if ((currbody & BD_FLAG_RED) && (currbody & BD_FLAG_GREEN)) currbody = BD_FLAG_BLUE;
	else currbody |= BD_FLAG_BLUE;
}

void GO_Ball::setAlive(const bool alive)
{
	this->alive = alive;
}
