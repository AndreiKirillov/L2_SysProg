#pragma once
#include "framework.h"

// �����, ����������� �������� � �������, ������ �������������, ����� ������ � ����� ������������ �������,
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

	bool Create(AFX_THREADPROC thread_function, LPVOID param);

};

