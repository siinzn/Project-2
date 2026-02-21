
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <cstdint>

uint64_t FromFileTime(const FILETIME ft) {
    ULARGE_INTEGER uli = { 0 };
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    return uli.QuadPart;
}


uint64_t getSysTime() {
    FILETIME idle, kernel, user;
    FILETIME idle_2, kernel_2, user_2;
    GetSystemTimes(&idle, &kernel, &user);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    GetSystemTimes(&idle_2, &kernel_2, &user_2);

    uint64_t idle0 = FromFileTime(idle);
    uint64_t idle1 = FromFileTime(idle_2);
    uint64_t kernel0 = FromFileTime(kernel);
    uint64_t kernel1 = FromFileTime(kernel_2);
    uint64_t user0 = FromFileTime(user);
    uint64_t user1 = FromFileTime(user_2);

    uint64_t totalIdle = idle1 - idle0;
    uint64_t totalKernel = kernel1 - kernel0;
    uint64_t totalUser = user1 - user0;

    uint64_t totalSys = totalKernel + totalUser;

    std::cout << "System Idle : " << totalIdle << std::endl;
    std::cout << "System Kernel : " << totalKernel << std::endl;
    std::cout << "System User : " << totalUser << std::endl;

    //uint64_t cpu = (totalSys - totalIdle) * 100 / (totalSys);
    return 0;
}


int main()
{
    getSysTime();
    //std::cout << "Raw CPU usage : " << getSysTime() << std::endl;
}

