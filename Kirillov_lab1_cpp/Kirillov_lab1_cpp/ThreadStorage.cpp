#include "pch.h"
#include "ThreadStorage.h"

ThreadStorage::ThreadStorage(): _threads(0), _activated_threads(0)
{
}

int ThreadStorage::GetCount()
{
	return _threads.size();
}

void ThreadStorage::AddThread(std::unique_ptr<ThreadKirillov>&& somethread)  // Добавление потока
{
	_threads.emplace_back(std::move(somethread));
}

void ThreadStorage::ActivateLastThread()           // Посылает сигнал активации последнему потоку
{
	if(_threads.size() > 0)
		_threads[_threads.size() - 1]->SetActive();
}

void ThreadStorage::DeleteLastThread()            
{
	// перемещаем последний поток в активированные
	_activated_threads.emplace_back(std::move(_threads[_threads.size() - 1]));
	// укорачиваем основное хранилище
	_threads.pop_back();
}

//void ThreadStorage::CreateNewThread(AFX_THREADPROC thread_function, ParamsToThread& param)
//{
//	_threads.emplace_back(std::make_unique<ThreadKirillov>());
//	_threads[0]->Create(thread_function, param);
//}
