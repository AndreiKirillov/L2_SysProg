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
	HANDLE _receive_msg_event;

	ThreadKirillov(ThreadKirillov&);            // запрещаем копирование и присваивание
	ThreadKirillov(const ThreadKirillov&);      // сделав конструкторы приватными
	ThreadKirillov& operator=(ThreadKirillov&);
public:
	ThreadKirillov();
	~ThreadKirillov();

	void SetID(int id);
	int GetID();
	
	bool Create(AFX_THREADPROC thread_function, ParamsToThread&& param);
	void SetActive();

	void ReceiveMessage();

	//bool Create(AFX_THREADPROC thread_function);
	

};

