#include "pch.h"
#include "EventsKirillov.h"

EventsKirillov::EventsKirillov(): _events(), _count(0)
{
}



void EventsKirillov::CreateNewEvent()     // ������ ����� �������
{
    string event_name = "EventID-" + to_string(_count);
    HANDLE h = CreateEvent(NULL, FALSE, FALSE, (LPCWSTR)event_name.c_str());
    _events.emplace(h);      // �������� ������� � ���������
    ++_count;
}

int EventsKirillov::GetCount()      // ���������� ���������� ��������� �������
{
    return _count;
}

HANDLE EventsKirillov::GetLastEvent()     // ���������� ��������� �������
{
    return _events.top();
}

void EventsKirillov::SetLastEvent()       // ���������� ��������� �������
{
    if (_events.empty())
        throw length_error("���� ������� ����!");

    if (SetEvent(_events.top()) == 0)
        throw exception("�� ������� ��������� ������� � ���������� ���������!");
}

void EventsKirillov::DeleteLastEvent()      // ������� ��������� �������, ����������� �������
{
    if (_events.empty())
        throw length_error("���� ������� ����!");

    if (CloseHandle(_events.top()) == FALSE)
        throw exception("�� ������� ������� ������ ����(�������)!");

    _events.pop();
    --_count;
}
