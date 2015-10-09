#pragma once

#include "a_gameobjectclass.h"
#include "go_upgradeclass.h"

#define BALL_INIT_TRANSL D3DXVECTOR3(0.0f, -7.66f, 0.0f)
#define BALL_INIT_VEL D3DXVECTOR3(0.0f, 0.2f, 0.0f)
#define BALL_CONST_ACCEL 1.01f
#define MAX_BOUNCES 180

class GO_Ball: public GameObject
{
public:
	GO_Ball();
	~GO_Ball();

	bool Initialize(
		ComPtr<ID3D11Device> const,
		const std::shared_ptr<ShaderManager>,
		const std::shared_ptr<MaterialFactory>);

	void Update();
	void Render(ComPtr<ID3D11DeviceContext> const, const D3DXMATRIX&, const D3DXVECTOR3&);

	void Upgrade(
		const upgr_flag,
		ComPtr<ID3D11Device> const,
		const std::shared_ptr<MaterialFactory>);

	void ReverseVel(const bool, const bool);
	
	bool getUnchained() const;
	bool getOnFire() const;
	bool getOnGrass() const;
	bool getOnWater() const;
	bool getAlive() const;
	D3DXVECTOR3 getVel() const;

	void setUnchained(const bool);
	void setOnFire();
	void setOnGrass();
	void setOnWater();
	void setAlive(const bool);
	void setVelX(const float);
	void ResetVel();
	void ResetBounces();

private:
	static std::map<unsigned int, char*> materials;

private:
	bool unchained;
	bool alive;
	unsigned int currbody;
	unsigned int bounces;
	D3DXVECTOR3 vel;
};