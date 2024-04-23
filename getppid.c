#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

#define FAILURE 0

DWORD getppid() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    DWORD ppid = 0, pid = GetCurrentProcessId();

    if (hSnapshot == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "CreateToolhelp32Snapshot failed.err = %lu\n",
                GetLastError());

        return FAILURE;
    }

    ZeroMemory(&pe32, sizeof(pe32));

    pe32.dwSize = sizeof(pe32);

    if (!Process32First(hSnapshot, &pe32)) {
        fprintf(stderr, "Process32First failed.err = %lu\n", GetLastError());
        CloseHandle(hSnapshot);

        return FAILURE;
    }

    do {
        if (pe32.th32ProcessID == pid) {
            ppid = pe32.th32ParentProcessID;

            break;
        }
    } while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);

    return ppid;
}
