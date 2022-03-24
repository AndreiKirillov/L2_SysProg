#include "pch.h"
#include "ThreadKirillov.h"

ThreadKirillov::ThreadKirillov(): _id(0), _thread(nullptr), _control_event(nullptr)
{
}

ThreadKirillov::~ThreadKirillov()
{
	CloseHandle(_thread);
	CloseHandle(_control_event);
}

bool ThreadKirillov::Create(AFX_THREADPROC thread_function, LPVOID param)
{
	ParamsToThread p;
	p.id = _id;

	_control_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	p.control_event = _control_event;

	_thread = AfxBeginThread(thread_function, &p)->m_hThread;

	return true;
}
