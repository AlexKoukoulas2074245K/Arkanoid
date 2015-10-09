#pragma once

// Custom includes
#include "common.h"
#include "d3drendererclass.h"
#include "modelclass.h"
#include "rasterfpsclass.h"
#include "rastercpuclass.h"
#include "cameraclass.h"
#include "inputclass.h"
#include "i_gamestateclass.h"
#include "a_gameobjectclass.h"
#include "go_ballclass.h"
#include "go_blockclass.h"
#include "go_boardclass.h"
#include "levelclass.h"
#include "physics.h"
#include "shadermanagerclass.h"
#include "materialfactoryclass.h"
#include "transitionclass.h"

// Projection Specifications
#define FRUSTANGLE static_cast<float>(D3DX_PI) / 4.0f
#define ZNEAR 0.1f
#define ZFAR 100.0f
#define ASPECTRATIO  static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT)

class Game : public GameState
{
public:
	Game();
	~Game();
	bool Initialize(const HWND&);
	void HandleInput(const MSG&);
	void Update();
	void Render();
	bool isFinished() const;

private:
	bool Initialize(const int);
	bool InitComponents();
	void UpdateWindowTitle();

private:
	HWND hWindow;
	bool finished;
	
	D3DXVECTOR4 clearColor;

	// Renderer
	D3DRenderer d3drend;

	// Math
	D3DXMATRIX projectionMatrix;
	
	// Input
	UserInput inputManager;
	Camera camera;
	FpsClass fpsCounter;
	CpuClass cpuCounter;

	// Factories
	std::shared_ptr<ShaderManager> shaderManager;
	std::shared_ptr<MaterialFactory> materialFactory;

	// Transitions
	Transition transition;

	// Entities
	std::shared_ptr<GO_Board> board;
	std::shared_ptr<GO_Ball> ball;
	std::vector<std::shared_ptr<GameObject>> entities;
	std::unique_ptr<Level> level;

	// Progress
	unsigned int currLevel;
};
