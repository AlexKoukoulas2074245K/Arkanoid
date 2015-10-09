#include "gamestatemanagerclass.h"

GameStateManager::GameStateManager()
{
	
}

GameStateManager::~GameStateManager()
{
	LOG("Deleting GSM...\n");
	while (!states.empty())
	{
		states.pop();
	}
}

bool GameStateManager::Initialize(const HWND& hwnd)
{
	states.push(std::unique_ptr<GameState>(new Game));
	if (!states.front()->Initialize(hwnd))
	{
		return false;
	}

	return true;
}

void GameStateManager::HandleInput(const MSG& msg)
{
	states.front()->HandleInput(msg);
}

void GameStateManager::Update()
{
	states.front()->Update();
	if (states.front()->isFinished())
	{
		states.pop();
	}
}

void GameStateManager::Render()
{
	states.front()->Render();
}

bool GameStateManager::isFinished()
{
	return states.empty();
}