// Webbed.Injector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <string>
#include <cstring>
#include <TlHelp32.h>
#include <easyhook.h>

#pragma comment(lib, "EasyHook32.lib")
#pragma comment(lib, "psapi.lib")

unsigned long WebbedProcessID = -1;

DWORD GetWebbedProcessID()
{
	HANDLE hProcSnap;
	PROCESSENTRY32 pe32;

	hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcSnap, &pe32))
	{
		std::cout << " Process32First failed " << std::endl;
		CloseHandle(hProcSnap);
		return -1;
	}

	do
	{
		std::wcout << "Found process: " << pe32.szExeFile << std::endl;

		if (lstrcmpiW(L"webbed.exe", pe32.szExeFile) == 0)
		{
			CloseHandle(hProcSnap);
			return  pe32.th32ProcessID;
		}
	} while (Process32Next(hProcSnap, &pe32));

	CloseHandle(hProcSnap);
	return (-1);
}

int main()
{
	WebbedProcessID = GetWebbedProcessID();

	if (WebbedProcessID == -1)
		return 1;

	const WCHAR* DynLib = L"Webbed.TAS.dll";

	NTSTATUS ntResult = RhInjectLibrary(WebbedProcessID,
		0, EASYHOOK_INJECT_DEFAULT, (WCHAR*)DynLib, nullptr, NULL, 0);

	if (ntResult != 0)
	{
		printf("Failed to inject with error code %d\n", ntResult);
		PWCHAR err = RtlGetLastErrorString();
		std::wcout << err << "\n";
	}

	else
	{
		std::cout << "Injection succesfull.\n";
	}


}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
