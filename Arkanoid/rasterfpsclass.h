#pragma once

#pragma comment(lib, "winmm.lib")

// Custon includes
#include <Windows.h>
#include <mmsyscom.h>
#include "common.h"

class FpsClass
{
public:
	FpsClass();
	~FpsClass();

	void Initialize();
	void Update();
	int GetFps();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};