#pragma once

#include <queue>
#include "i_gamestateclass.h"
#include "gs_gameclass.h"

class GameStateManager
{
public:
	GameStateManager();
	~GameStateManager();

	bool Initialize(const HWND&);
	void HandleInput(const MSG&);
	void Update();
	void Render();
	bool isFinished();

private:
	// Polymorphic GameState queue
	std::queue<std::unique_ptr<GameState>> states;
};