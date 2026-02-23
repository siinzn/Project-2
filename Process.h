#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <vector>
#include <string>
struct ProcessInfo
{
	DWORD pId;
	std::wstring fileName;
};

HANDLE getSnapshot();
std::vector<ProcessInfo> getProcess(HANDLE process);
void printProcess();
