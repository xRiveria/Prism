#include "PrismPrecompiledHeader.h"
#include "Threading.h"

namespace Prism
{
	Threading::Threading()
	{
		m_Stopping = false;
		m_SupportedThreadCount = std::thread::hardware_concurrency();

		m_ThreadCount = m_SupportedThreadCount - 1; //<- Wtf

		m_ThreadNames[std::this_thread::get_id()] = "Main"; //Store our main thread.

		for (size_t i = 0; i < m_ThreadCount; i++)
		{
			m_Threads.emplace_back(std::thread(&Threading::ThreadLoop, this));
			m_ThreadNames[m_Threads.back().get_id()] = "Worker_" + std::to_string(i);
		}
		
		PRISM_ENGINE_INFO("{0} threads have been created.", m_ThreadCount);
	}

	Threading::~Threading()
	{
		FlushTasks(true);

		//Put a unique lock on our task mutex.
		std::unique_lock<std::mutex> taskLock(m_TaskMutex);

		//Set termination flag to true.
		m_Stopping = true;

		//Unlock the mutex.
		taskLock.unlock();

		//Wake up all threads.
		m_ThreadCondition.notify_all();

		//Join all threads.
		for (auto& thread : m_Threads)
		{
			thread.join();
		}

		//Empty out all workers threads.
		m_Threads.clear();
	}

	uint32_t Threading::GetThreadsAvaliable() const
	{
		uint32_t avaliableThreads = m_ThreadCount;
		for (const auto& task : m_Tasks)
		{
			avaliableThreads -= task->IsTaskExecuting() ? 1 : 0;
		}

		return avaliableThreads;
	}

	void Threading::FlushTasks(bool removeQueue /*= false*/)
	{
		//Clear any queued tasks.
		if (removeQueue == true)
		{
			m_Tasks.clear();
		}

		//If any tasks are running, we wait for them.
		while (AreTasksRunning())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	}

	void Threading::ThreadLoop()
	{
		Reference<Task> task;
		while (true)
		{
			//Lock Tasks Mutex
			std::unique_lock<std::mutex> taskLock(m_TaskMutex);

			//Check condition on notification.
			m_ThreadCondition.wait(taskLock, [this] { return !m_Tasks.empty() || m_Stopping; }); //Does not proceed until the condition returns true.

			//If m_Stopping is true, it's time to shut everything down.
			if (m_Stopping && m_Tasks.empty())
			{
				return;
			}

			//Else, we get next task in the queue.
			task = m_Tasks.front();

			//Remove it from the queue.
			m_Tasks.pop_front();

			//Unlock the mutex.
			taskLock.unlock();

			//Execute the task.
			task->Execute();
		}
	}
}