#include "pch.h"
#include "ThreadStorage.h"

ThreadStorage::ThreadStorage(): _threads(0)
{
}

void ThreadStorage::CreateNewThread(AFX_THREADPROC thread_function, ParamsToThread&& param)
{
	_threads.emplace_back(std::make_unique<ThreadKirillov>());
	_threads[0]->Create(thread_function, std::move(param));
}
