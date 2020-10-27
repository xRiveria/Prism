#pragma once

//ImGui Instrumentor System

/* 
template<typename Function>
class Timer
{
public:
	Timer(const char* timerName, Function&& function) : m_TimerName(timerName), m_Stopped(false), m_Function(function)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
		{
			Stop();
		}
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		auto duration = (end - start) * 0.001f;

		m_Function({ m_TimerName, duration });
	}

private:
	const char* m_TimerName;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
	Function m_Function;
};

#define PROFILING_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

example:

void Play()
{
	PROFILING_SCOPE("Sandbox2D::OnUpdate");
	...
}
*/