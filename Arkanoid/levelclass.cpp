#include "levelclass.h"

Level::Level()
{

}

Level::~Level()
{

}

bool Level::Initialize(
	const LevelLayout& layout,
	ComPtr<ID3D11Device> const device,
	const std::shared_ptr<ShaderManager> shaderManager,
	const std::shared_ptr<MaterialFactory> materialFactory)
{
	level.clear();
	upgrades.clear();
	currSize = 0;
	initSize = 0;
	animatingCol = 0;
	animatingRow = 0;

	PreBuffering(device, shaderManager, materialFactory);
	background = std::unique_ptr<GO_Background>(new GO_Background);
	if (!background->Initialize(device, shaderManager, materialFactory))
	{
		return false;
	}

	lvlRows = layout.structure.size();
	lvlCols = layout.structure[0].size();
	layoutXOffset = layout.xOffset;
	layoutYOffset = layout.yOffset;
	lvlEffect = layout.effectType;

	for (size_t y = 0; y < layout.structure.size(); y++)
	{
		level.push_back(std::vector<std::unique_ptr<GO_Block>>());
	
		for (size_t x = 0; x < layout.structure[y].size(); x++)
		{
			level[y].push_back(std::unique_ptr<GO_Block>(new GO_Block));
			float blockX = layoutXOffset - x * LVL_HOR_BLOCK_DIST;
			float blockY = layoutYOffset - y * LVL_VER_BLOCK_DIST;
			int blockType = layout.structure[y][x];

			if (!level[y][x]->Initialize(
				device,
				blockX,
				blockY,
				blockType,
				shaderManager,
				materialFactory))
			{
				return false;
			}

			initSize++;
		}
	}

	currSize = initSize;

	return true;
}

void Level::Update(
	const std::shared_ptr<GO_Ball> ball,
	const std::shared_ptr<GO_Board> board,
	ComPtr<ID3D11Device> const device,
	const std::shared_ptr<ShaderManager> shaderManager,
	const std::shared_ptr<MaterialFactory> matFactory)
{
	background->Update();

	bool ballCollided = false;
	for (size_t y = 0; y < level.size(); y++)
	{
		for (size_t x = 0; x < level[y].size(); x++)
		{
			EffectHandling(x, y);
			level[y][x]->Update();

			if (!level[y][x]->getAlive())
			{
				level[y].erase(level[y].begin() + x);
				currSize--;
				continue;
			}
			
			if (level[y][x]->getHit())
			{
				continue;
			}

			int clresult = collides(
				SHAPE_CIRCLE,
				ball->getTransformation(),
				ball->getDimensions(),
				SHAPE_RECTANGLE,
				level[y][x]->getTransformation(),
				level[y][x]->getDimensions());
			
			if (clresult && !ballCollided)
			{
				level[y][x]->Hit();

				if (!ball->getOnWater())
				{
					clresult == COLL_HOR ? ball->ReverseVel(true, false) :
										   ball->ReverseVel(false, false);
					ballCollided = true;
				}
				
				if (ball->getOnFire())
				{
					HitSurroundings(level[y][x]->getTranslation().x,
								    level[y][x]->getTranslation().y);
				}

				if (level[y][x]->getHit()) // May not be the case if the blocks are invisible
				{
					UpgradeHandling(
						x, y,
						device,
						shaderManager,
						matFactory);
				}
			}
		}
	}

	for (size_t i = 0; i < upgrades.size(); i++)
	{
		upgrades[i]->Update();

		if (collides(
			SHAPE_RECTANGLE,
			upgrades[i]->getTransformation(),
			upgrades[i]->getDimensions(),
			SHAPE_RECTANGLE,
			board->getTransformation(),
			board->getDimensions()))
		{
			ball->Upgrade(upgrades[i]->getFlag(), device, matFactory);
			upgrades[i]->Hit();
		}

		if (!upgrades[i]->getAlive()) upgrades.erase(upgrades.begin() + i);
	}

}

void Level::Render(
	ComPtr<ID3D11DeviceContext> const devcon,
	const D3DXMATRIX& viewProjection,
	const D3DXVECTOR3& camPos)
{
	//background->Render(devcon, viewProjection, camPos);

	for (size_t i = 0; i < level.size(); i++)
	{
		for (size_t y = 0; y < level[i].size(); y++)
		{
			if (level[i][y]->getInvisible()) continue;
			level[i][y]->Render(devcon, viewProjection, camPos);
		}
	}

	for (size_t i = 0; i < upgrades.size(); i++)
	{
		upgrades[i]->Render(devcon, viewProjection, camPos);
	}
}

bool Level::getFinished() const
{
	return currSize == 0;
}

void Level::HitSurroundings(const float blockX, const float blockY)
{
	for (size_t y = 0; y < level.size(); y++)
	{
		for (size_t x = 0; x < level[y].size(); x++)
		{
			float xDist = std::abs(level[y][x]->getTranslation().x - blockX);
			float yDist = std::abs(level[y][x]->getTranslation().y - blockY);
			float xDiff = std::abs(level[y][x]->getDimensions().width - xDist);
			float yDiff = std::abs(level[y][x]->getDimensions().height - yDist);

			if (xDiff < level[y][x]->getDimensions().width / 2 &&
				yDist == 0.0) level[y][x]->Hit();
			if (xDist == 0 &&
				yDiff < (3 * level[y][x]->getDimensions().height) / 2) level[y][x]->Hit();
		}
	}
}

void Level::EffectHandling(
	const size_t x,
	const size_t y)
{
	switch (lvlEffect)
	{
		case 1:
		{
			level[y][x]->Animate(360.0f);
		}break;

		case 2:
		{
			if (x == animatingCol)
			{
				level[y][x]->Animate(180.0f);
				if (level[y][x]->getAniFinish())
				{
					animatingCol = (animatingCol + 1) % lvlCols;
				}
			}
		}break;
	}
}

void Level::UpgradeHandling(
	const size_t x, const size_t y,
	ComPtr<ID3D11Device> const device,
	const std::shared_ptr<ShaderManager> shaderManager,
	const std::shared_ptr<MaterialFactory> matFactory)
{
	if (std::rand() % UPGR_CHANCE == 1)
	{
		upgrades.push_back(std::unique_ptr<GO_Upgrade>(new GO_Upgrade));
		
		D3DXVECTOR3 upgrPos(level[y][x]->getTranslation());

		if (!upgrades[upgrades.size() - 1]->Initialize(device, upgrPos, shaderManager, matFactory))
		{
			MessageBox(NULL, "Upgrade creation failed", FATALERROR, FATALLAYOUT);
			return;
		}
	}
}

void Level::PreBuffering(
	ComPtr<ID3D11Device> const device,
	const std::shared_ptr<ShaderManager> shaderManager,
	const std::shared_ptr<MaterialFactory> matFactory)
{
	GO_Upgrade garbage;
	garbage.Initialize(device, D3DXVECTOR3(), shaderManager, matFactory);
	matFactory->getMaterial(device, "/textures/ball/ballbase.dds");
	matFactory->getMaterial(device, "/textures/ball/ballred.dds");
	matFactory->getMaterial(device, "/textures/ball/ballblue.dds");
	matFactory->getMaterial(device, "/textures/ball/ballgreen.dds");
	matFactory->getMaterial(device, "/textures/ball/ballpink.dds");
	matFactory->getMaterial(device, "/textures/ball/ballcyan.dds");
	matFactory->getMaterial(device, "/textures/ball/ballyellow.dds");
	matFactory->getMaterial(device, "/textures/upgrades/upgrade_red.dds");
	matFactory->getMaterial(device, "/textures/upgrades/upgrade_green.dds");
	matFactory->getMaterial(device, "/textures/upgrades/upgrade_blue.dds");
}