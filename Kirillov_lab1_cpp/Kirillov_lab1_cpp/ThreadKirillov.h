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
public:
	ThreadKirillov();
	~ThreadKirillov();

	void SetID(int id);
	void SetControlEvent(const HANDLE& event);
	
	bool Create(AFX_THREADPROC thread_function, ParamsToThread&& param);
	//bool Create(AFX_THREADPROC thread_function);
	

};

