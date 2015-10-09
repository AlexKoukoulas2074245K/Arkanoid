#pragma once

// Linking
#pragma comment(lib, "pdh.lib")

// Includes
#include <Pdh.h>
#include "common.h"

class CpuClass
{
public:
	CpuClass();
	~CpuClass();

	void Initialize();
	void Update();
	
	int GetCpuPercentage();

private:
	bool m_canReadCpu;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;
	long m_cpuUsage;
};