#pragma once
#include "framework.h"

using namespace std;

class EventsKirillov   //Класс для хранения о обработки событий(объектов ядра)
{
private:
	vector<HANDLE> _threads;
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

