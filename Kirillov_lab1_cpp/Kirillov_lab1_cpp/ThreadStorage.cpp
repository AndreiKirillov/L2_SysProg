#include "pch.h"
#include "ThreadStorage.h"

ThreadStorage::ThreadStorage(): _threads(0), _finished_threads(0)
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

void ThreadStorage::FinishLastThread()           // Посылает сигнал активации последнему потоку
{
	if(_threads.size() > 0)
		_threads[_threads.size() - 1]->SetActive();
}

void ThreadStorage::DeleteLastThread()            
{
	// перемещаем последний поток в активированные
	_finished_threads.emplace_back(std::move(_threads[_threads.size() - 1]));
	// укорачиваем основное хранилище
	_threads.pop_back();
}

void ThreadStorage::DeleteAll()   // очищает все потоки, у них отрабатывает деструктор, освобождая ресурсы
{
	_threads.clear();
	_finished_threads.clear();
}

void ThreadStorage::ActionThreadByID(int id)
{
	auto t = find_if(_threads.begin(), _threads.end(), [&](auto& some_thread) {return some_thread->GetID() == id; });

	if (t != _threads.end())
		t->get()->ReceiveMessage();
}

void ThreadStorage::ActionLastThread()
{
	if (_threads.size() > 0)
		_threads[_threads.size() - 1]->ReceiveMessage();
}


