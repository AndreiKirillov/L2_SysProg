#pragma once
#include "framework.h"
#include "ThreadKirillov.h"

// �����, ����������� ������� ������ � �������� � ����
class ThreadStorage
{
private:
	std::vector<std::unique_ptr<ThreadKirillov>> _threads;

	std::vector<std::unique_ptr<ThreadKirillov>> _finished_threads;
public:
	ThreadStorage();

	int GetCount();

	void AddThread(std::unique_ptr<ThreadKirillov>&& somethread);

	void FinishLastThread();

	void DeleteLastThread();
	void DeleteAll();

	void ActionLastThread();

};

