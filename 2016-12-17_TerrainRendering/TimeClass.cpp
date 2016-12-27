#include "TimeClass.h"

TimeClass::TimeClass()
{
}

TimeClass::TimeClass(const TimeClass &other)
{
}

TimeClass::~TimeClass()
{
}

bool TimeClass::Initialize()
{
	INT64 frequency;

	QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
	
	if (frequency == 0)
		return false;

	m_frequency = (float)frequency;

	QueryPerformanceCounter((LARGE_INTEGER *)&m_startTime);

	return true;
}

void TimeClass::Frame()
{
	INT64 currentTime;
	INT64 elapsedTicks;

	QueryPerformanceCounter((LARGE_INTEGER *)&currentTime);

	elapsedTicks = currentTime - m_startTime;
	m_frameTime = (float)elapsedTicks / m_frequency;
	m_startTime = currentTime;

	return;
}

float TimeClass::GetTime()
{
	return m_frameTime;
}

void TimeClass::StartTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER *)&m_beginTime);

	return;
}

void TimeClass::StopTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER *)&m_endTime);

	return;
}

int TimeClass::GetTiming()
{
	float elapsedTicks;
	INT64 frequency;
	float milliseconds;

	elapsedTicks = (float)(m_endTime - m_beginTime);

	QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);

	milliseconds = (elapsedTicks / (float)frequency) * 1000.0f;

	return (int)milliseconds;
}
