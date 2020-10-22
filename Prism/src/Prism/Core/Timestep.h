#pragma once

namespace Prism
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) : m_Time(time)
		{

		}
		
		operator float() { return m_Time; } //Cast operator for the class if you try to cast it to a float, for example.

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }
	
	private:
		float m_Time;
	};
}