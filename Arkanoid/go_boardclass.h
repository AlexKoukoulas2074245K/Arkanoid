#pragma once
#include "a_gameobjectclass.h"

#define MOUSE_DELTA_TOL 85.0f
#define BOARD_INIT_TRANSL D3DXVECTOR3(0.0f, -8.5f, 0.0f)
#define BOARD_INIT_SCALE D3DXVECTOR3(0.6f, 0.6f, 0.6f)

class GO_Board: public GameObject
{
public:
	GO_Board();
	~GO_Board();

	bool Initialize(
		ComPtr<ID3D11Device> const,
		const std::shared_ptr<ShaderManager>,
		const std::shared_ptr<MaterialFactory>);

	void Update();
	void Render(ComPtr<ID3D11DeviceContext> const, const D3DXMATRIX&, const D3DXVECTOR3&);
	void Move(const float);
};