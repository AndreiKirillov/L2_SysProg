#include "pch.h"
#include "ThreadKirillov.h"

ThreadKirillov::ThreadKirillov(): _id(0), _thread(nullptr), _control_event(nullptr)
{
}

ThreadKirillov::~ThreadKirillov()
{
	/*if(_thread != nullptr)
		CloseHandle(_thread);
	if(_control_event != nullptr)
		CloseHandle(_control_event);*/
}

void ThreadKirillov::SetID(int id)
{
	_id = id;
}

void ThreadKirillov::SetControlEvent(const HANDLE& event)
{
	_control_event = event;
}

bool ThreadKirillov::Create(AFX_THREADPROC thread_function, ParamsToThread&& param)
{
	_id = param.id;
	_control_event = param.control_event;
	
	_thread = AfxBeginThread(thread_function, std::move(&param))->m_hThread;

	return true;
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
