#pragma once
#include "Prism/Core/Core.h"
#include "ThreadTask.h"
#include <condition_variable>
#include <thread>
#include <deque>
#include <utility>
#include <vector>
#include <memory>

namespace Prism
{
	class Threading
	{
	public:
		Threading();
		~Threading();

		//Add a task.
		template<typename Function>
		void AddTask(Function&& function)
		{
			if (m_Threads.empty()) //If no threads are avaliable.
			{
				PRISM_CLIENT_WARN("No avaliable threads for multithreading. Function will execute in the same thread.");
				function();
				return;
			}

			//Lock task mutex.
			std::unique_lock<std::mutex> taskLocker(m_TaskMutex);

			//Save the task.
			m_Tasks.push_back(CreateReference<Task>(std::bind(std::forward<Function>(function))));

			//Unlock the task.
			taskLocker.unlock();

			//Wake up a thread to handle the task.
			m_ThreadCondition.notify_one(); //Notify a thread to wake up and handle the task.
		}

		//Adds a task which is a loop and executes chunks of it in parallel.
		template <typename Function>
		void AddTaskLoop(Function&& function, uint32_t loopRange)
		{
			uint32_t avaliableThreads = GetThreadsAvaliable();
			std::vector<bool> tasksCompleted = std::vector<bool>(avaliableThreads, false);
			const uint32_t taskCount = avaliableThreads + 1; //Plus one for the current thread.

			uint32_t start = 0;
			uint32_t end = 0;

			for (uint32_t = 0; i < avaliableThreads; i++)
			{
				start = (loopRange / taskCount) * i;
				end = start + (loopRange / taskCount);
				
				//Kick off task.
				AddTask([&function, &tasksCompleted, i, start, end]
				{
					function(start, end);
					tasksCompleted[i] = true;
				});
			}

			//Do task in the current thread.
			function(end, loopRange);

			//Wait till the threads are done.
			uint32_t tasks = 0;
			while (tasks != tasksCompleted.size())
			{
				tasks = 0;
				for (const bool jobDone : tasksCompleted)
				{
					tasks += jobDone ? 1 : 0;
				}
			}
		}

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
		std::deque<Reference<Task>> m_Tasks; //Tasks we have to run.

		std::mutex m_TaskMutex;
		std::condition_variable m_ThreadCondition;
		bool m_Stopping;
	};

}
