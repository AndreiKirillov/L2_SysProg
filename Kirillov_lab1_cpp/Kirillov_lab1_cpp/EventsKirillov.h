#pragma once
#include "framework.h"

using namespace std;

class EventsKirillov   //����� ��� �������� � ��������� �������(�������� ����)
{
private:
	stack<HANDLE, list<HANDLE>> _events;
	int _count;
public:
	EventsKirillov();

	void CreateNewEvent();

	int GetCount();

	HANDLE GetLastEvent();

	void SetLastEvent();

	void DeleteLastEvent();
};

