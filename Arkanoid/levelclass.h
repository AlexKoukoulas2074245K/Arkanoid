#pragma once

#include <vector>
#include "common.h"
#include "go_blockclass.h"
#include "go_ballclass.h"
#include "go_boardclass.h"
#include "go_upgradeclass.h"
#include "go_backgroundclass.h"
#include "ioutils.h"
#include "physics.h"
#include "materialfactoryclass.h"

#define LVL_HOR_BLOCK_DIST 3.1f
#define LVL_VER_BLOCK_DIST 1.2f


class Level
{
public:

	Level();
	~Level();
	bool Initialize(
		const LevelLayout&,
		ComPtr<ID3D11Device> const,
		const std::shared_ptr<ShaderManager>,
		const std::shared_ptr<MaterialFactory>);

	void Update(
		const std::shared_ptr<GO_Ball>,
		const std::shared_ptr<GO_Board>,
		ComPtr<ID3D11Device> const,
		const std::shared_ptr<ShaderManager> shaderManager,
		const std::shared_ptr<MaterialFactory> matFactory);

	void Render(ComPtr<ID3D11DeviceContext> const, const D3DXMATRIX&, const D3DXVECTOR3&);
	bool getFinished() const;

private:
	void EffectHandling(const size_t, const size_t);
	void UpgradeHandling(
		const size_t,
		const size_t,
		ComPtr<ID3D11Device> const,
		const std::shared_ptr<ShaderManager>,
		const std::shared_ptr<MaterialFactory>);
	void HitSurroundings(const float, const float);
	void PreBuffering(
		ComPtr<ID3D11Device> const,
		const std::shared_ptr<ShaderManager>,
		const std::shared_ptr<MaterialFactory>);

private:
	std::vector<std::vector<std::unique_ptr<GO_Block>>> level;
	std::vector<std::unique_ptr<GO_Upgrade>> upgrades;
	std::unique_ptr<GO_Background> background;
	
	float layoutXOffset;
	float layoutYOffset;
	unsigned int lvlEffect;
	unsigned int lvlRows;
	unsigned int lvlCols;
	unsigned int initSize;
	unsigned int currSize;

	size_t animatingCol;
	size_t animatingRow;
};