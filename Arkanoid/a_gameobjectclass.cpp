#include "a_gameobjectclass.h"

GameObject::GameObject()
{
	ZeroMemory(&transform, sizeof(Model::Transform));
	transform.scaling.x = 1.0f;
	transform.scaling.y = 1.0f;
	transform.scaling.z = 1.0f;
}

bool GameObject::Initialize(
	ComPtr<ID3D11Device> const device,
	const std::shared_ptr<ShaderManager> shaderManager,
	const std::shared_ptr<MaterialFactory> materialFactory)
{
	this->shaderManager = shaderManager;
	return true;
}

void GameObject::Update()
{
	model.Update(transform);
}

void GameObject::Render(
	ComPtr<ID3D11DeviceContext> const devcon,
	const D3DXMATRIX& viewProjection,
	const D3DXVECTOR3& camPos)
{
	model.Render(devcon, viewProjection);
}

Model::Dimensions GameObject::getDimensions() const
{
	return model.getCurrentDimensions();
}

Model::Transform GameObject::getTransformation() const
{
	return transform;
}

D3DXVECTOR3 GameObject::getTranslation() const
{
	return transform.translation;
}

D3DXVECTOR3 GameObject::getRotation() const
{
	return transform.rotation;
}

D3DXVECTOR3 GameObject::getScale() const
{
	return transform.scaling;
}

void GameObject::setTranslation(const float x, const float y, const float z) 
{
	transform.translation = D3DXVECTOR3(x, y, z); 
}

void GameObject::setTranslation(const D3DXVECTOR3 translation)
{
	this->transform.translation = translation;
}

void GameObject::setRotation(const float rx, const float ry, const float rz)
{
	transform.rotation = D3DXVECTOR3(rx, ry, rz);
}

void GameObject::setScale(const float sx, const float sy, const float sz)
{
	transform.scaling = D3DXVECTOR3(sx, sy, sz);
	Model::Dimensions initDims = model.getInitialDimensions();
	model.setDimensions(initDims.depth * sz, initDims.height * sy, initDims.width * sx);
}

void GameObject::setScale(const float scale)
{
	setScale(scale, scale, scale);
}

void GameObject::TranslateBy(const float x, const float y, const float z)
{
	transform.translation.x += x;
	transform.translation.y += y;
	transform.translation.z += z;
}

void GameObject::RotateBy(const float rx, const float ry, const float rz)
{
	transform.rotation.x += rx;
	transform.rotation.y += ry;
	transform.rotation.z += rz;
}

void GameObject::ScaleBy(const float sx, const float sy, const float sz)
{
	transform.scaling.x += sx;
	transform.scaling.y += sy;
	transform.scaling.z += sz;

	Model::Dimensions currDims = model.getCurrentDimensions();
	model.setDimensions(
		currDims.depth + currDims.depth * sz,
		currDims.height + currDims.height * sy,
		currDims.width + currDims.width * sx);
}

void GameObject::ScaleBy(const float scale)
{
	ScaleBy(scale, scale, scale);
}