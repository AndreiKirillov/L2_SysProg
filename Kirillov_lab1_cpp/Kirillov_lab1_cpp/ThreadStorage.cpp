#include "pch.h"
#include "ThreadStorage.h"

ThreadStorage::ThreadStorage(): _threads(0), _activated_threads(0)
{
}

int ThreadStorage::GetCount()
{
	return _threads.size();
}

void ThreadStorage::AddThread(std::unique_ptr<ThreadKirillov>&& somethread)  // ���������� ������
{
	_threads.emplace_back(std::move(somethread));
}

void ThreadStorage::ActivateLastThread()           // �������� ������ ��������� ���������� ������
{
	if(_threads.size() > 0)
		_threads[_threads.size() - 1]->SetActive();
}

void ThreadStorage::DeleteLastThread()            
{
	// ���������� ��������� ����� � ��������������
	_activated_threads.emplace_back(std::move(_threads[_threads.size() - 1]));
	// ����������� �������� ���������
	_threads.pop_back();
}

//void ThreadStorage::CreateNewThread(AFX_THREADPROC thread_function, ParamsToThread& param)
//{
//	_threads.emplace_back(std::make_unique<ThreadKirillov>());
//	_threads[0]->Create(thread_function, param);
//}
