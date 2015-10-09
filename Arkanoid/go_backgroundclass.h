#pragma once
#include "a_gameobjectclass.h"

#define BG_MATERIAL "/textures/background2.dds"
#define BG_MODEL "/models/plane.obj"

class GO_Background: public GameObject
{
public:
	GO_Background();
	~GO_Background();

	bool Initialize(
		ComPtr<ID3D11Device> const,
		const std::shared_ptr<ShaderManager>,
		const std::shared_ptr<MaterialFactory>);
	void Update();
	void Render(
		ComPtr<ID3D11DeviceContext> const,
		const D3DXMATRIX&,
		const D3DXVECTOR3&);
};