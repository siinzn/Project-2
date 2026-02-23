#include "Process.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>
#include <vector>



HANDLE getSnapshot() {
    HANDLE hProcess;
    DWORD dwError = 0;
    hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcess == INVALID_HANDLE_VALUE) {
        std::cout << "Error : " << GetLastError() << std::endl;
    }
    return hProcess;
}

std::vector<ProcessInfo> getProcess(HANDLE process) {
    std::vector<ProcessInfo> processes;
    PROCESSENTRY32 pe32;

    pe32.dwSize = sizeof(PROCESSENTRY32);
    BOOL Process32F = Process32First(process, &pe32);

    if (!Process32F) {
        std::cout << "Error : " << GetLastError() << std::endl;
        CloseHandle(process);
        return processes;
    }

    do {
        ProcessInfo pi;
        pi.pId = pe32.th32ProcessID;
        pi.fileName = std::wstring(pe32.szExeFile);
        processes.push_back(pi);
    } while (Process32Next(process, &pe32));
    CloseHandle(process);
    return processes;
}

void printProcess() {
    HANDLE snapshot = getSnapshot();
    std::vector<ProcessInfo> processes = getProcess(snapshot);
    for (auto p : processes) {
        std::wcout << p.fileName << L" - " << p.pId << std::endl;
    }
}