#include "rastercpuclass.h"

CpuClass::CpuClass()
{

}

CpuClass::~CpuClass()
{
	LOG("Deleting CpuClass...\n");
	if (m_canReadCpu)
	{
		PdhCloseQuery(m_queryHandle);
	}
}

void CpuClass::Initialize()
{
	PDH_STATUS status;

	// Initialize the flag indicating whether this object can read the system cpu usage or not
	m_canReadCpu = true;

	// Create a query object to pull cpu usage.
	status = PdhOpenQuery(NULL, NULL, &m_queryHandle);
	if (status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	// Set query object to pull all cpus in the system.
	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
	if (status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	m_lastSampleTime = GetTickCount();

	m_cpuUsage = 0;
}

void CpuClass::Update()
{
	PDH_FMT_COUNTERVALUE value;

	if (m_canReadCpu)
	{
		if ((m_lastSampleTime + 1000) < GetTickCount())
		{
			m_lastSampleTime = GetTickCount();
			PdhCollectQueryData(m_queryHandle);
			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);
			m_cpuUsage = value.longValue;
		}
	}
}

int CpuClass::GetCpuPercentage()
{
	if (m_canReadCpu)
	{
		return m_cpuUsage;
	}
	
	return 0;
}