#include "PrismPrecompiledHeader.h"
#include "Threading.h"

namespace Prism
{
	Threading::Threading()
	{
		m_Stopping = false;
		m_SupportedThreadCount = std::thread::hardware_concurrency();

		m_ThreadCount = m_SupportedThreadCount - 1; //Minus 1 for our main thread.

		m_ThreadNames[std::this_thread::get_id()] = "Main"; //Store our main thread.

		for (size_t i = 0; i < m_ThreadCount; i++)
		{
			m_Threads.emplace_back(std::thread(&Threading::ThreadLoop, this)); //This will create our threads and associate them with a thread of execution. Execution starts immediately. 
			m_ThreadNames[m_Threads.back().get_id()] = "Worker_" + std::to_string(i); //Associates our threads with worker names.
		}
		
		PRISM_ENGINE_INFO("{0} threads have been created.", m_ThreadCount);
	}

	Threading::~Threading() //Should only be called by the main thread.
	{
		FlushTasks(true); //If our threading instance is going to be destroyed, we flush and forcibly remove all queued tasks.

		//Put a unique lock on our task mutex.
		std::unique_lock<std::mutex> taskLock(m_TaskMutex);

		//Set termination flag to true.
		m_Stopping = true;

		//Unlock the mutex.
		taskLock.unlock();

		//Alert all threats. They will return as our termination flag is true.
		m_ThreadCondition.notify_all(); 

		//Join all threads. The main thread will wait for all of them to finish execution.
		for (auto& thread : m_Threads)
		{
			thread.join();
		}

		//Delete all thread instances.
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

	void Threading::FlushTasks(bool removeQueue /*= false*/) //This should and will only be called by the main thread.
	{
		//Forcibly clear queued tasks if true.
		if (removeQueue == true)
		{
			m_Tasks.clear();
		}

		//If any tasks are running, we wait for them to complete.
		while (AreTasksRunning())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(16)); //Make our main thread sleep while our sub threads finish their work.
		}
	}

	void Threading::ThreadLoop()
	{
		Reference<Task> task;

		while (true) //Check every frame.
		{
			//Put a lock here to prevent data racing. 
			std::unique_lock<std::mutex> taskLock(m_TaskMutex);

			//Thread is put to sleep until the condition below returns true. 
			m_ThreadCondition.wait(taskLock, [this] { return !m_Tasks.empty() || m_Stopping; }); //If there are tasks to be run, or if the threading system is stopped, return true to execute our tasks.

			//If our threading system is halted or if tasks are empty, return from our loop.
			if (m_Stopping && m_Tasks.empty())
			{
				return;
			}

			//Alrighty, we have a task and execution is ready. Retrieve the task.
			task = m_Tasks.front();

			//Remove it from the queue.
			m_Tasks.pop_front();

			//Unlock our lock so another thread can check for further tasks.
			taskLock.unlock();

			//Execute the retrieved task.
			task->Execute();
		}
	}
}