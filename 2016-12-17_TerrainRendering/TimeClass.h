#ifndef _TIMECLASS_H_
#define _TIMECLASS_H_

#include <Windows.h>

class TimeClass
{
public:
	TimeClass();
	TimeClass(const TimeClass &);
	~TimeClass();

	bool Initialize();
	void Frame();

	float GetTime();

	void StartTimer();
	void StopTimer();
	int GetTiming();

private:
	float m_frequency;
	INT64 m_startTime;
	float m_frameTime;
	INT64 m_beginTime, m_endTime;
};

#endif