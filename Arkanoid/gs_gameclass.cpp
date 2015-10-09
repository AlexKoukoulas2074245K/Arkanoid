#include "gs_gameclass.h"

Game::Game()
{

}

Game::~Game()
{
	LOG("Deleting Game...\n");
}

bool Game::Initialize(const int level)
{
	if (!InitComponents())
	{
		finished = true;
		return false;
	}

	return true;
}

bool Game::Initialize(const HWND& hwnd)
{
	hWindow = hwnd;
	finished = false;
	clearColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0);
	currLevel = 0;

	// 3D Renderer Initialization
	if (!d3drend.Initialize(hWindow))
	{
		return false;
	}

	// Shader Manager initialization
	shaderManager = std::shared_ptr<ShaderManager>(new ShaderManager);
	if (!shaderManager->Initialize(d3drend.getDevice()))
	{
		return false;
	}

	// Material Factory initialization
	materialFactory = std::shared_ptr<MaterialFactory>(new MaterialFactory);

	// Game Component Initialization
	if (!InitComponents())
	{
		return false;
	}

	return true;
}


bool Game::InitComponents()
{	

	// Level Initialization
	level = std::unique_ptr<Level>(new Level);
	LevelLayout lvlLayout = {};
	
	std::string levelPath = "/levels/level";
	levelPath += std::to_string(currLevel);
	levelPath += ".lvl";

	if (!LoadLevelFromFile(GetAssetPath(levelPath).c_str(), lvlLayout))
	{
		return false;
	}

	if (!level->Initialize(lvlLayout, d3drend.getDevice(), shaderManager, materialFactory))
	{
		return false;
	}

	// Entity initialization
	ball = std::shared_ptr<GO_Ball>(new GO_Ball);
	board = std::shared_ptr<GO_Board>(new GO_Board);

	entities.clear();
	entities.push_back(ball);
	entities.push_back(board);

	for (size_t i = 0; i < entities.size(); i++)
	{
		if (!entities[i]->Initialize(d3drend.getDevice(), shaderManager, materialFactory))
		{
			return false;
		}
	}

	// Set camera
	D3DXVECTOR3 cameraPos = D3DXVECTOR3(0.0f, 0.0f, 25.0f);
	camera.Initialize(cameraPos);

	// Set projection matrix
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, FRUSTANGLE, ASPECTRATIO, ZNEAR, ZFAR);
	
	// Transitions
	if (!transition.Initialize(d3drend.getDevice(), shaderManager))
	{
		return false;
	}

	// Set Debug Counters
	cpuCounter.Initialize();
	fpsCounter.Initialize();

	return true;
}

bool Game::isFinished() const
{
	return finished;
}

void Game::HandleInput(const MSG& msg)
{
	inputManager.UpdateState(msg, hWindow);
}

void Game::Update()
{
	UpdateWindowTitle();
	if (transition.getActive())
	{
		transition.Update();
		if (transition.getFinished())
		{
			currLevel = (currLevel + 1) % 2;
			Initialize(currLevel);
		}
		return;
	}

	if (inputManager.getKey(K_LEFT)) camera.Look(Camera::Direction::LEFT, 0.03f);
	if (inputManager.getKey(K_RIGHT)) camera.Look(Camera::Direction::RIGHT, 0.03f);

	if (inputManager.getMouse().lButton && !ball->getUnchained())
	{
		ball->setUnchained(true);
		ball->setVelX((ball->getTranslation().x - board->getTranslation().x) / 8.0f);
	}
	
	// Entities are updated here
	for (size_t i = 0; i < entities.size(); ++i)
	{
		entities[i]->Update();
	}

	if (!ball->getAlive())
	{
		ball->Initialize(d3drend.getDevice(), shaderManager, materialFactory);
		ball->setTranslation(board->getTranslation().x, 0.0f, 0.0f);
	}

	// Chained phase
	float boardX0 = board->getTranslation().x;
	board->Move(inputManager.getMouse().dx);
	float boardDx = board->getTranslation().x - boardX0;

	if (!ball->getUnchained())
	{
		ball->setTranslation(
			ball->getTranslation().x,
			BALL_INIT_TRANSL.y,
			ball->getTranslation().z);

		ball->TranslateBy(boardDx, 0.0f, 0.0f);
	}

	if (collides(
		Shape::SHAPE_CIRCLE,
		ball->getTransformation(),
		ball->getDimensions(),
		Shape::SHAPE_RECTANGLE,
		board->getTransformation(),
		board->getDimensions()) && ball->getVel().y < 0)
	{
		if (ball->getOnGrass())
		{
			ball->setUnchained(false);
			ball->ResetBounces();
		}
		else
		{
			ball->ReverseVel(false, true);
			ball->setVelX((ball->getTranslation().x - board->getTranslation().x) / 8.0f);
		}
	}

	level->Update(ball, board, d3drend.getDevice(), shaderManager, materialFactory);
	if (level->getFinished()) transition.setActive(true);

	camera.Update();
	inputManager.ResetMouse(hWindow);
	
}

void Game::Render()
{
	/* --------- Depth Included Rendering ------------ */
	d3drend.BeginRendering(true);
	d3drend.ClearScreen(clearColor, true);

	D3DXMATRIX transCamView = camera.getViewMatrix();
	D3DXVECTOR3 transCamPos = camera.getPosition();
	D3DXMATRIX viewProjection = transCamView * projectionMatrix;

	// Entities are Rendered here
	for (size_t i = 0; i < entities.size(); ++i)
	{
		entities[i]->Render(d3drend.getDeviceContext(), viewProjection, transCamPos);
	}

	level->Render(d3drend.getDeviceContext(), viewProjection, transCamPos);

	/* --------- Depth Ignored Rendering ------------ */
	transition.Render(d3drend.getDeviceContext(), shaderManager);

	d3drend.SwapBuffers();
}

void Game::UpdateWindowTitle()
{
	fpsCounter.Update();
	cpuCounter.Update();

	std::string stats = "Fps: " + std::to_string(fpsCounter.GetFps()) +
		" - Cpu Usage: " + std::to_string(cpuCounter.GetCpuPercentage()) + "%";
	SetWindowText(hWindow, stats.c_str());
}

