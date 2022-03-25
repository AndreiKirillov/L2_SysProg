#pragma once
#include "framework.h"
#include "ThreadKirillov.h"

// �����, ����������� ������� ������ � �������� � ����
class ThreadStorage
{
private:
	std::vector<std::unique_ptr<ThreadKirillov>> _threads;

	std::vector<std::unique_ptr<ThreadKirillov>> _activated_threads;
public:
	ThreadStorage();

	int GetCount();

	void AddThread(std::unique_ptr<ThreadKirillov>&& somethread);

	void ActivateLastThread();

	void DeleteLastThread();

	//void CreateNewThread(AFX_THREADPROC thread_function, ParamsToThread& param);

};

