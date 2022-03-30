// FileMapping.cpp: определяет процедуры инициализации для библиотеки DLL.
//

#include "pch.h"
#include "framework.h"
#include "FileMapping.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: если эта библиотека DLL динамически связана с библиотеками DLL MFC,
//		все функции, экспортированные из данной DLL-библиотеки, которые выполняют вызовы к
//		MFC, должны содержать макрос AFX_MANAGE_STATE в
//		самое начало функции.
//
//		Например:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// тело нормальной функции
//		}
//
//		Важно, чтобы данный макрос был представлен в каждой
//		функции до вызова MFC.  Это означает, что
//		должен стоять в качестве первого оператора в
//		функции и предшествовать даже любым объявлениям переменных объекта,
//		поскольку их конструкторы могут выполнять вызовы к MFC
//		DLL.
//
//		В Технических указаниях MFC 33 и 58 содержатся более
//		подробные сведения.
//

// CFileMappingApp

BEGIN_MESSAGE_MAP(CFileMappingApp, CWinApp)
END_MESSAGE_MAP()


// Создание CFileMappingApp

CFileMappingApp::CFileMappingApp()
{
	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
}


// Единственный объект CFileMappingApp

CFileMappingApp theApp;


// Инициализация CFileMappingApp

BOOL CFileMappingApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

struct header
{
	int thread_id;
	int message_size;
};

struct message
{
	int thread_id;
	char* message;

};

HANDLE hFile;
HANDLE hFileMap;

extern "C"
{
	__declspec(dllexport) bool __stdcall CreateMappingFile()
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		hFile = CreateFileA("myfile.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
		if (hFile == INVALID_HANDLE_VALUE)
			return false;

		return true;
	}

	__declspec(dllexport) bool __stdcall SendMappingMessage(void* message, header& h)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		hFileMap = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, h.message_size + sizeof(header), NULL);
		char* buff = (char*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, h.message_size + sizeof(header));
		memcpy(buff, &h, sizeof(header));
		//buff = const_cast<char*>(message.c_str());
		memcpy(buff + sizeof(header), message, h.message_size);
		//memcpy(buff + sizeof(header), const_cast<char*>(message.c_str()), h.message_size);

		UnmapViewOfFile(buff);
		return true;

		//AFX_MANAGE_STATE(AfxGetStaticModuleState());
		//HANDLE hFileMap = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, h.message_size + sizeof(header), NULL);
		//char* buff = (char*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, h.message_size + sizeof(header));

		//memcpy(buff, &h, sizeof(header));
		////buff = const_cast<char*>(message.c_str());
		//memcpy(buff + sizeof(header), const_cast<char*>(message.c_str()), h.message_size);
		////memcpy(buff + sizeof(header), const_cast<char*>(message.c_str()), h.message_size);

		//UnmapViewOfFile(buff);
		//return true;
	}

	__declspec(dllexport) char* __stdcall ReadMessage(header& h)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		hFileMap = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, h.message_size + sizeof(header), NULL);
		char* buff_for_header = (char*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(header));//h.message_size + sizeof(header));
		memcpy(&h, buff_for_header, sizeof(header));
		UnmapViewOfFile(buff_for_header);
		CloseHandle(hFileMap);

		hFileMap = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, h.message_size + sizeof(header), NULL);
		char* buff_for_msg = (char*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(header)+ h.message_size);

		char* message = new char[h.message_size];
		memcpy(message, buff_for_msg + sizeof(header), h.message_size);

		UnmapViewOfFile(buff_for_msg);
		return message;

		//AFX_MANAGE_STATE(AfxGetStaticModuleState());
		//hFileMap = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, sizeof(header), NULL);
		//char* buff = (char*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(header));//h.message_size + sizeof(header));
		////char* message = new char[strlen(buff)];

		//memcpy(&h, buff, sizeof(header));
		//UnmapViewOfFile(buff);             
		//CloseHandle(hFileMap);           // возможно это глупо

		//hFileMap = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, sizeof(header) + h.message_size, NULL);
		//char* new_buff = (char*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(header) + h.message_size);
		//char* ch_message = new char[h.message_size];
		//memcpy(ch_message, new_buff + sizeof(header), h.message_size);

		//UnmapViewOfFile(new_buff);
		//message = ch_message;
		//delete[] ch_message;

		
	}

	__declspec(dllexport) void __stdcall CloseFileMapping()
	{
		CloseHandle(hFileMap);
		CloseHandle(hFile);
	}

}
