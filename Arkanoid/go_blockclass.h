#pragma once

#include "a_gameobjectclass.h"

#define BLK_RAD_ROT static_cast<float>(D3DXToRadian(1.0f))

class GO_Block: public GameObject
{
public:
	GO_Block();
	~GO_Block();

	bool Initialize(
		ComPtr<ID3D11Device> const,
		const float,
		const float,
		const int,
		const std::shared_ptr<ShaderManager>,
		const std::shared_ptr<MaterialFactory>);

	void Update();
	void Render(ComPtr<ID3D11DeviceContext> const, const D3DXMATRIX&, const D3DXVECTOR3&);
	
	bool getInvisible() const;
	bool getAlive() const;
	bool getHit() const; 
	bool getAniFinish() const;

	void Hit();
	void Animate(const float);

private:
	bool hit;
	bool alive;
	bool invisible;
	bool aniFinished;
	float alphaCounter;
};