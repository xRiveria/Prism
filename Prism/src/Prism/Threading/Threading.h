#pragma once
#include <condition_variable>
#include <thread>
#include <deque>
#include <utility>

namespace Prism
{
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

	class Threading
	{
	public:
		Threading();
		~Threading();

		//Get the number of threads currently used.
		uint32_t GetThreadCount() const { return m_ThreadCount; }

		//Get the maximum number of threads the hardware support.
		uint32_t GetSupportedThreadCount() const { return m_SupportedThreadCount; } //Get the maximum number of threads the hardware supports.

		//Get the number of threads which are not doing any work.
		uint32_t GetThreadsAvaliable() const;

		//Returns true if at least one task is running.
		bool AreTasksRunning() const { return GetThreadsAvaliable() != GetThreadCount(); } //The not- equal - to operator ( != ) returns true if the operands don't have the same value; otherwise, it returns false .
		
		//Wait for all executing (and queued if requested) tasks to finish. 
		void FlushTasks(bool removeQueue = false);

	private:
		//This function is invoked by threads.
		void ThreadLoop();

		uint32_t m_ThreadCount;
		uint32_t m_SupportedThreadCount;

		std::vector<std::thread> m_Threads;
		std::unordered_map<std::thread::id, std::string> m_ThreadNames;
		std::deque<Reference<Task>> m_Tasks;

		std::mutex m_TaskMutex;
		std::condition_variable m_ThreadCondition;
		bool m_Stopping;
	};

}
