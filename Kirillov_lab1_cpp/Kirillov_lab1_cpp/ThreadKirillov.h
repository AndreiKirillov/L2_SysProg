#pragma once
#include "framework.h"

// �����, �������������� ������ ������ ������ ��� ������ � ���, ������ �������������, ����� ������ � ����� ������������ �������,
// ������� ��������� ���������� ������
class ThreadKirillov
{
private:
	int _id;
	HANDLE _thread;
	HANDLE _control_event;
	HANDLE _receive_msg_event;

	ThreadKirillov(ThreadKirillov&);            // ��������� ����������� � ������������
	ThreadKirillov(const ThreadKirillov&);      // ������ ������������ ����������
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

