﻿// Kirillov_lab1_cpp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include "framework.h"
#include "Kirillov_lab1_cpp.h"
#include "EventsKirillov.h"
#include "ThreadKirillov.h"
#include "ThreadStorage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Единственный объект приложения

CWinApp theApp;

using namespace std;

UINT ThreadFunction(LPVOID param)     // Функция для выполнения в потоке
{
    ParamsToThread* p = static_cast<ParamsToThread*>(param);
    string n = to_string(p->id);

    cout << "Thread №" + n + " START" << endl;
    WaitForSingleObject(p->control_event, INFINITE);     // Ждём сигнал от события
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

            HANDLE error_event = CreateEventA(NULL, FALSE, FALSE, "ErrorEvent");
            kernel_objects.push_back(error_event);

            HANDLE hMutex = CreateMutexA(NULL, FALSE, "MyMutex");

            HANDLE hControlEvents[3] = { create_thread_event, close_thread_event, close_programm_event };

            EventsKirillov events;      // события для потоков
            ThreadStorage storage;
            SetEvent(confirm_event);   // подтвердение запуска приложения

            while (true)
            {
                int event_index = WaitForMultipleObjects(3, hControlEvents, FALSE, INFINITE) - WAIT_OBJECT_0; // Ждём событие от 
                                                                                                              // главной программы
                switch (event_index)
                {
                case 0:         // Событие создания потока
                {
                    std::unique_ptr<ThreadKirillov> t = std::make_unique<ThreadKirillov>();
                    
                    ParamsToThread p;
                    p.id = storage.GetCount() + 1;
                    p.control_event = CreateEventA(NULL, FALSE, FALSE, NULL);
                    if (p.control_event == NULL)
                    {
                        SetEvent(error_event);
                        break;
                    }

                    if (!t->Create(ThreadFunction, std::move(p)))
                    {
                        SetEvent(error_event);
                        break;
                    }
                    
                    storage.AddThread(std::move(t));
                    SetEvent(confirm_event);
                }
                break;

                case 1:              // Событие завершения потока
                {
                    if (storage.GetCount() > 0)
                    {
                        storage.ActivateLastThread();
                        storage.DeleteLastThread();
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

    return nRetCode;
}
