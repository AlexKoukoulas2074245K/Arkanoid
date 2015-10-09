#pragma once
#include "a_gameobjectclass.h"

typedef unsigned short upgr_type;
typedef unsigned int upgr_flag;

#define BD_FLAG_NONE 0x00
#define BD_FLAG_RED 0x01
#define BD_FLAG_GREEN 0x02
#define BD_FLAG_BLUE 0x04

#define UPT_RED 0x00
#define UPT_GREEN 0x01
#define UPT_BLUE 0x02
#define N_UPTYPES 0x03
#define UPGR_CHANCE 0x0f
#define UPGR_KILL_Y -10.0f
#define RED_MAT "/textures/upgrades/upgrade_red.dds"
#define GREEN_MAT "/textures/upgrades/upgrade_green.dds"
#define BLUE_MAT "/textures/upgrades/upgrade_blue.dds"

class GO_Upgrade: public GameObject
{
public:
	GO_Upgrade();
	~GO_Upgrade();

	bool Initialize(
		ComPtr<ID3D11Device> const,
		const D3DXVECTOR3,
		const std::shared_ptr<ShaderManager>,
		const std::shared_ptr<MaterialFactory>);

	void Update();
	void Render(ComPtr<ID3D11DeviceContext> const, const D3DXMATRIX&, const D3DXVECTOR3&);
	
	void Hit();
	
	upgr_type getType() const;
	upgr_flag getFlag() const;

	bool getAlive() const;

private:
	static char* materials[N_UPTYPES];

private:
	bool alive;
	upgr_type upgrType;
	upgr_flag upgrFlag;
};