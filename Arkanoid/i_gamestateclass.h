#pragma once

#include "common.h"

class GameState
{
public:
	virtual ~GameState(){};
	virtual bool Initialize(const HWND& hwnd) = 0;
	virtual void HandleInput(const MSG& msg) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual bool isFinished() const = 0;
};