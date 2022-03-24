#pragma once
#include "framework.h"
#include "ThreadKirillov.h"

class ThreadStorage
{
private:
	std::vector<std::unique_ptr<ThreadKirillov>> _threads;
public:
	ThreadStorage();

	void CreateNewThread(AFX_THREADPROC thread_function, ParamsToThread&& param);

};

