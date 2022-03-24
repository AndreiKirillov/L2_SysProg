// Kirillov_lab1_cpp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include "framework.h"
#include "Kirillov_lab1_cpp.h"
#include "EventsKirillov.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Единственный объект приложения

CWinApp theApp;

using namespace std;

DWORD WINAPI ThreadFunction(LPVOID param)     // Функция для выполнения в потоке
{
    EventsKirillov* event = static_cast<EventsKirillov*>(param);
    string n = to_string(event->GetCount());

    cout << "Thread №" + n + " START" << endl;
    WaitForSingleObject(event->GetLastEvent(), INFINITE);     // Ждём сигнал от события
    cout << "Thread №" + n + " CLOSE" << endl;
    return 0;
}

void CloseAllObjects(list<HANDLE> handles)    // Освобождение русурсов всех объектов ядра
{
    for (auto event : handles)
    {
        CloseHandle(event);
    }
}

BOOL WINAPI ConsoleControl(DWORD dwType)
{
    switch (dwType)
    {
        case CTRL_CLOSE_EVENT:
        {
            HANDLE user_close_programm = CreateEventA(NULL, FALSE, FALSE, "UserCloseProgramm");
            SetEvent(user_close_programm);
            return TRUE;
        }
        default:
            return FALSE;
    }
}


int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // инициализировать MFC, а также печать и сообщения об ошибках про сбое
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: вставьте сюда код для приложения.
            wprintf(L"Критическая ошибка: сбой при инициализации MFC\n");
            nRetCode = 1;
        }
        else
        {
            setlocale(LC_ALL, "Russian");

            // список программных событий
            list<HANDLE> kernel_objects; 
            HANDLE create_thread_event = CreateEventA(NULL, FALSE, FALSE, "CreateNewThread");
            kernel_objects.push_back(create_thread_event);

            HANDLE close_thread_event = CreateEventA(NULL, FALSE, FALSE, "CloseThread");
            kernel_objects.push_back(close_thread_event);

            HANDLE confirm_event = CreateEventA(NULL, FALSE, FALSE, "ConfirmEvent");
            kernel_objects.push_back(confirm_event);

            HANDLE close_programm_event = CreateEventA(NULL, FALSE, FALSE, "CloseProgrammEvent");
            kernel_objects.push_back(close_programm_event);

            HANDLE hControlEvents[3] = { create_thread_event, close_thread_event, close_programm_event };

            EventsKirillov events;      // события для потоков

            while (true)
            {
                int event_index = WaitForMultipleObjects(3, hControlEvents, FALSE, INFINITE) - WAIT_OBJECT_0; // Ждём событие от 
                                                                                                              // главной программы
                switch (event_index)
                {
                case 0:         // Событие создания потока
                {
                    events.CreateNewEvent();
                    CreateThread(NULL, 0, ThreadFunction, &events, 0, NULL);
                    SetEvent(confirm_event);
                }
                break;

                case 1:              // Событие завершения потока
                {
                    if (events.GetCount() > 0)
                    {
                        events.SetLastEvent();
                        events.DeleteLastEvent();
                        SetEvent(confirm_event);
                    }
                    else      // Освобождаем все ресурсы, если нет активных потоков
                    {
                        SetEvent(close_programm_event);
                        CloseAllObjects(kernel_objects);

                        return 0;
                    }
                }
                break;

                case 2:      // Закрытие программы
                {
                    SetEvent(close_programm_event);
                    CloseAllObjects(kernel_objects);

                    return 0;
                }
                }
            }

        }
    }
    else
    {
        // TODO: измените код ошибки в соответствии с потребностями
        wprintf(L"Критическая ошибка: сбой GetModuleHandle\n");
        nRetCode = 1;
    }

    if (SetConsoleCtrlHandler(ConsoleControl, TRUE))
        return 0;

    return nRetCode;
}
