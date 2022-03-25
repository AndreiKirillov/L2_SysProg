#include "pch.h"
#include "ThreadKirillov.h"

ThreadKirillov::ThreadKirillov(): _id(0), _thread(nullptr), _control_event(nullptr)
{
}

ThreadKirillov::~ThreadKirillov() // В деструкторе освобождаем ресурсы потока
{
	std::cout << "DESTRUCTOR" << std::endl;
	/*if(_thread != NULL)        
		CloseHandle(_thread);*/
	if(_control_event != nullptr)
		CloseHandle(_control_event);
}

void ThreadKirillov::SetID(int id)
{
	_id = id;
}

// Функция создания потока, возвращает false при некорректной работе
bool ThreadKirillov::Create(AFX_THREADPROC thread_function, ParamsToThread&& param)
{
	if (param.id == 0 || param.control_event == NULL)
		return false;
	_id = param.id;
	_control_event = param.control_event;
	
	CWinThread* new_thread = AfxBeginThread(thread_function, std::move(&param));
	if (new_thread == NULL)
		return false;
	_thread = new_thread->m_hThread;

	return true;
}

void ThreadKirillov::SetActive() // Посылает сигнал для активизации потока
{
	if(_control_event != nullptr)
		SetEvent(_control_event);
}

//bool ThreadKirillov::Create(AFX_THREADPROC thread_function)
//{
//	ParamsToThread p;
//	p.id = _id;
//	if(_control_event != nullptr)
//		p.control_event = _control_event;
//	else
//	{
//		_control_event = CreateEvent(NULL, FALSE, FALSE, NULL);
//		p.control_event = _control_event;
//	}
//
//	_thread = AfxBeginThread(thread_function, &p)->m_hThread;
//
//	return true;
//}
