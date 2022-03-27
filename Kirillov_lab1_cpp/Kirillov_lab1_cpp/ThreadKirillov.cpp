#include "pch.h"
#include "ThreadKirillov.h"

ThreadKirillov::ThreadKirillov(): _id(0), _thread(nullptr), _control_event(nullptr), _receive_msg_event(nullptr)
{
}

ThreadKirillov::~ThreadKirillov() // � ����������� ����������� ������� ������
{
	std::cout <<"ID "<< std::to_string(_id)<< " DESTRUCTOR" << std::endl;
	/*if(_thread != NULL)        
		CloseHandle(_thread);*/
	if(_control_event != nullptr)
		CloseHandle(_control_event);
}

void ThreadKirillov::SetID(int id)
{
	_id = id;
}

int ThreadKirillov::GetID()
{
	return _id;
}

// ������� �������� ������, ���������� false ��� ������������ ������
bool ThreadKirillov::Create(AFX_THREADPROC thread_function, ParamsToThread&& param)
{
	if (param.id == 0 || param.control_event == NULL || param.receive_msg_event == NULL)
		return false;
	_id = param.id;
	_control_event = param.control_event;
	_receive_msg_event = param.receive_msg_event;
	
	CWinThread* new_thread = AfxBeginThread(thread_function, std::move(&param));
	if (new_thread == NULL)
		return false;
	_thread = new_thread->m_hThread;

	return true;
}

void ThreadKirillov::SetActive() // �������� ������ ��� ����������� ������
{
	if(_control_event != nullptr)
		SetEvent(_control_event);
}

void ThreadKirillov::ReceiveMessage()
{
	if (_receive_msg_event != nullptr)
		SetEvent(_receive_msg_event);
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
