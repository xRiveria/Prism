#pragma once
#include <iostream>
#include <utility>
#include <functional>
#include <thread>

class Task
{
public:
	Task(std::function<void()>&& function)
	{
		m_Function = std::forward<std::function<void()>>(function);
	}

	void Execute()
	{
		std::cout << "Executing on Thread ID: " << std::this_thread::get_id() << "\n";
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