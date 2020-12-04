#pragma once
#include "Prism/Core/Core.h"
#include "ThreadTask.h"
#include <condition_variable>
#include <thread>
#include <deque>
#include <utility>
#include <vector>
#include <memory>

/* Threading System Summary

- All threads the hardware supports are created and put to sleep on initialization.
- If the threading instance is destroyed, all tasks are destroyed, m_Stopping is set to true, all threads are woken up and executions ended and all threads are destroyed.
- We can use m_Stopping = true to halt all thread executions or resume them.
- FlushTasks allows us to either forcibly remove all tasks or make our main thread sleep until all sub threads complete task execution.
- When each thread instance is created, they are put to sleep immediately and only activated when condition_variable is notified and tasks are found to be in the queue, in which execution will proceed.

*/

namespace Prism
{
	class Threading
	{
	public:
		Threading();
		~Threading();

		//Add a task. Takes in a function object.
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

			//Unlock the task mutex.
			taskLocker.unlock();

			//Wake up a thread to handle the task.
			m_ThreadCondition.notify_one(); 
		}

		//Adds a task which is a loop and executes chunks of it in parallel.
		template <typename Function>
		void AddTaskLoop(Function&& function, uint32_t loopRange)
		{
			uint32_t avaliableThreads = GetThreadsAvaliable();
			std::vector<bool> tasksCompleted = std::vector<bool>(avaliableThreads, false); //Create a bool vector with size of all currently avaliable threads and initialize their values to false.
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

		//Get the threads avaliable for us to use. This does not include the main thread.
		uint32_t GetThreadCount() const { return m_ThreadCount; }

		//Get the maximum number of threads the hardware support.
		uint32_t GetSupportedThreadCount() const { return m_SupportedThreadCount; } 

		//Get the number of threads which are not doing any work.
		uint32_t GetThreadsAvaliable() const;

		//Returns true if at least one task is running.
		bool AreTasksRunning() const 
		{ 
			return GetThreadsAvaliable() != GetThreadCount(); //Returns true if both aren't equal, else returns false.
		} 
		
		//Clear all leftover tasks whether through force removal or wait.
		void FlushTasks(bool removeQueue = false);

	private:
		//This function is invoked by threads.
		void ThreadLoop();

		uint32_t m_ThreadCount;
		uint32_t m_SupportedThreadCount;

		std::vector<std::thread> m_Threads; //Create our threads.
		std::unordered_map<std::thread::id, std::string> m_ThreadNames; //Map our threads to names.
		std::deque<Reference<Task>> m_Tasks; //Tasks we have to run.

		std::mutex m_TaskMutex;
		std::condition_variable m_ThreadCondition;
		bool m_Stopping;
	};

}
