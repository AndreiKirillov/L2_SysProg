#pragma once
#include "framework.h"

//  ласс, представл€ющий обЄртку вокруг потока дл€ работы с ним, хранит идентификатор, хэндл потока и хэндл управл€ющего событи€,
// которое управл€ет поведением потока
class ThreadKirillov
{
private:
	int _id;
	HANDLE _thread;
	HANDLE _control_event;
public:
	ThreadKirillov();
	~ThreadKirillov();

	void SetID(int id);
	void SetControlEvent(const HANDLE& event);
	
	bool Create(AFX_THREADPROC thread_function, ParamsToThread&& param);
	//bool Create(AFX_THREADPROC thread_function);
	

};

