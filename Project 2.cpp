
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <cstdint>
#include <vector>

uint64_t FileTimeToInt64(const FILETIME ft) {
    ULARGE_INTEGER uli = { 0 };
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    return uli.QuadPart;
}

struct FILETIME_as_int
{
    uint64_t idle;
    uint64_t kernel;
    uint64_t user;
};

struct cpuDeltas
{
    uint64_t dIdle;
    uint64_t dKernel;
    uint64_t dUser;
    uint64_t dSystem;
};

FILETIME_as_int systemData() {

    FILETIME_as_int fi;
    FILETIME idle, kernel, user;
    GetSystemTimes(&idle, &kernel, &user);
    fi.idle = FileTimeToInt64(idle);
    fi.kernel = FileTimeToInt64(kernel);
    fi.user = FileTimeToInt64(user);

    if (!GetSystemTimes(&idle, &kernel, &user)) return FILETIME_as_int{ 0,0,0 };
    return fi;
}

cpuDeltas systemDataToInt(FILETIME_as_int previous, FILETIME_as_int current) {

    cpuDeltas dv;
    dv.dIdle = current.idle - previous.idle;
    dv.dKernel = current.kernel - previous.kernel;
    dv.dUser = current.user - previous.user;
    dv.dSystem = dv.dKernel + dv.dUser;
    
    return dv;
}

void printSystemData(cpuDeltas dv) {
    std::cout << "Idle : " << dv.dIdle << std::endl;
    std::cout << "Kernel : " << dv.dKernel << std::endl;
    std::cout << "User : " << dv.dUser << std::endl;
    uint64_t cpu_percent = (dv.dSystem - dv.dIdle) * 100 / dv.dSystem;
    std::cout << "System : " << cpu_percent << "%" << std::endl;
}


int main()
{
    FILETIME_as_int first_run = systemData();
    int cpu_runs = 10;
    for(int i{0}; i < cpu_runs; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        FILETIME_as_int second_run = systemData();

        cpuDeltas deltas = systemDataToInt(first_run, second_run);
        system("cls");
        printSystemData(deltas);
        first_run = second_run;
    }
    
}

