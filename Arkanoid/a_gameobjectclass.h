#pragma once

#include "common.h"
#include "modelclass.h"
#include "shadermanagerclass.h"
#include "materialfactoryclass.h"
#include "d3drendererclass.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject(){};

	virtual bool Initialize(
		ComPtr<ID3D11Device> const,
		const std::shared_ptr<ShaderManager>,
		const std::shared_ptr<MaterialFactory>);

	virtual void Update();
	virtual void Render(ComPtr<ID3D11DeviceContext> const, const D3DXMATRIX&, const D3DXVECTOR3&);

	Model::Dimensions getDimensions() const;
	Model::Transform getTransformation() const;
	D3DXVECTOR3 getTranslation() const;
	D3DXVECTOR3 getRotation() const;
	D3DXVECTOR3 getScale() const;

	void setTranslation(const float x, const float y, const float z);
	void setTranslation(const D3DXVECTOR3 translation);
	void setRotation(const float rx, const float ry, const float rz);
	void setScale(const float sx, const float sy, const float sz);
	void setScale(const float scale);
	void TranslateBy(const float x, const float y, const float z);
	void RotateBy(const float rx, const float ry, const float rz);
	void ScaleBy(const float sx, const float sy, const float sz);
	void ScaleBy(const float scale);

protected:
	Model model;
	Model::Transform transform;
	std::shared_ptr<ShaderManager> shaderManager;
};