#pragma once
#include "framework.h"

//  ласс, позвол€ющий работать с потоком, хранит идентификатор, хэндл потока и хэндл управл€ющего событи€,
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

	bool Create(AFX_THREADPROC thread_function, LPVOID param);

};

