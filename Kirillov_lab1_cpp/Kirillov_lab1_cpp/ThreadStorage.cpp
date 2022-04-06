#include "pch.h"
#include "ThreadStorage.h"

ThreadStorage::ThreadStorage(): _threads(0), _finished_threads(0)
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

void ThreadStorage::FinishLastThread()           // �������� ������ ��������� ���������� ������
{
	if(_threads.size() > 0)
		_threads[_threads.size() - 1]->SetActive();
}

void ThreadStorage::DeleteLastThread()            
{
	// ���������� ��������� ����� � ��������������
	_finished_threads.emplace_back(std::move(_threads[_threads.size() - 1]));
	// ����������� �������� ���������
	_threads.pop_back();
}

void ThreadStorage::DeleteAll()   // ������� ��� ������, � ��� ������������ ����������, ���������� �������
{
	_threads.clear();
	_finished_threads.clear();
}

void ThreadStorage::ActionAll()
{
	for (auto& thrd : _threads)
		thrd->ReceiveMessage();
}

void ThreadStorage::ActionThreadByID(int id)
{
	auto t = find_if(_threads.begin(), _threads.end(), [&](auto& some_thread) {return some_thread->GetID() == id; });

	if (t != _threads.end())
		t->get()->ReceiveMessage();
	else
		throw std::out_of_range("Error! Thread with ID = " + std::to_string(id) + " doesn't exist!");
}

void ThreadStorage::ActionLastThread()
{
	if (_threads.size() > 0)
		_threads[_threads.size() - 1]->ReceiveMessage();
}


