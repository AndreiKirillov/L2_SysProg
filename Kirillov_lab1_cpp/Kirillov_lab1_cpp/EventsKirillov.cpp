#include "pch.h"
#include "EventsKirillov.h"

EventsKirillov::EventsKirillov(): _events(), _count(0)
{
}



void EventsKirillov::CreateNewEvent()     // Создаёт новое событие
{
    string event_name = "EventID-" + to_string(_count);
    HANDLE h = CreateEvent(NULL, FALSE, FALSE, (LPCWSTR)event_name.c_str());
    _events.emplace(h);      // помещаем событие в хранилище
    ++_count;
}

int EventsKirillov::GetCount()      // Возвращает количество созданных событий
{
    return _count;
}

HANDLE EventsKirillov::GetLastEvent()     // Возвращает последнее событие
{
    return _events.top();
}

void EventsKirillov::SetLastEvent()       // Активирует последнее событие
{
    if (_events.empty())
        throw length_error("Стэк событий пуст!");

    if (SetEvent(_events.top()) == 0)
        throw exception("Не удалось перевести событие в сигнальное состояние!");
}

void EventsKirillov::DeleteLastEvent()      // Удаляет последнее событие, освобождает ресурсы
{
    if (_events.empty())
        throw length_error("Стэк событий пуст!");

    if (CloseHandle(_events.top()) == FALSE)
        throw exception("Не удалось закрыть объект ядра(событие)!");

    _events.pop();
    --_count;
}
