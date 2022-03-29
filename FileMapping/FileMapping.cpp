// FileMapping.cpp: определяет процедуры инициализации для библиотеки DLL.
//

#include "pch.h"
#include "framework.h"
#include "FileMapping.h"

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

HANDLE hFile;
HANDLE hFileMap;

extern "C"
{
	__declspec(dllexport) bool __stdcall CreateMappingFile(char* filename)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		//header h = { addr, strlen(str) + 1 };
		hFile = CreateFile((LPCWSTR)filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
		if (hFile == INVALID_HANDLE_VALUE)
			return false;

		/*hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, h.size + sizeof(header), NULL);
		if (hFileMap == NULL)
			return false;

		char* buff = (char*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, h.size + sizeof(header));

		memcpy(buff, &h, sizeof(header));
		memcpy(buff + sizeof(header), str, h.size);


		UnmapViewOfFile(buff);
		CloseHandle(hFileMap);
		CloseHandle(hFile);*/

		return true;
	}

	__declspec(dllexport) bool __stdcall SendMessage(char* message, header& h)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		//header h = { addr, strlen(str) + 1 };
		//header h = { addr, strlen(str) + 1 };
		HANDLE hFile = CreateFile((LPCWSTR)"file.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
		HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, h.message_size + sizeof(header), NULL);
		char* buff = (char*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, h.message_size + sizeof(header));

		memcpy(buff, &h, sizeof(header));
		memcpy(buff + sizeof(header), message, h.message_size);


		UnmapViewOfFile(buff);
		//CloseHandle(hFileMap);
		//CloseHandle(hFile);
	}

	__declspec(dllexport) char* __stdcall ReadMessage(header& h)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		HANDLE hFile = CreateFile((LPCWSTR)"file.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
		HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, h.message_size + sizeof(header), NULL);
		char* buff = (char*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, h.message_size + sizeof(header));

		memcpy(&h, buff, sizeof(header));
		char* message = nullptr;
		memcpy(message, buff + sizeof(header), h.message_size);


		UnmapViewOfFile(buff);
		//CloseHandle(hFileMap);
		//CloseHandle(hFile);
	}

}
