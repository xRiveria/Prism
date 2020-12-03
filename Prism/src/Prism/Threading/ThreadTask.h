#pragma once
#include <utility>
#include <functional>

class Task
{
public:
	Task(std::function<void()>&& function)
	{
		m_Function = std::forward<std::function<void()>>(function);
	}

	void Execute()
	{
		m_IsTaskExecuting = true;
		m_Function();
		m_IsTaskExecuting = false;
	}

	bool IsTaskExecuting() const
	{
		return m_IsTaskExecuting;
	}

private:
	std::function<void()> m_Function;
	bool m_IsTaskExecuting = false;
};