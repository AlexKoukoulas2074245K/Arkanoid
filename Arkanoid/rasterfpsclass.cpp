#include "rasterfpsclass.h"

FpsClass::FpsClass()
{
	
}

FpsClass::~FpsClass()
{
	LOG("Deleting FPSClass...\n");
}

void FpsClass::Initialize()
{
	m_fps = 0;
	m_count = 0;
	m_startTime = timeGetTime();
	return;
}

void FpsClass::Update()
{
	m_count++;

	if (timeGetTime() >= (m_startTime + 1000))
	{
		m_fps = m_count;
		m_count = 0;
		m_startTime = timeGetTime();
	}
}

int FpsClass::GetFps()
{
	return m_fps;
}