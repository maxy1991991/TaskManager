#include "process_management.h"
#include <tlhelp32.h>
#include <psapi.h>
#include <stdio.h>
#include <string.h>

int GetProcessList(ProcessInfo** processArray, int* arrayCount) {
    HANDLE snapshotHandle;
    PROCESSENTRY32 processEntry;
    int count = 0;
    ProcessInfo* localArray = NULL;

    snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshotHandle == INVALID_HANDLE_VALUE) {
        return 0;
    }

    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(snapshotHandle, &processEntry)) {
        CloseHandle(snapshotHandle);
        return 0;
    }

    do {
        HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processEntry.th32ProcessID);
        SIZE_T memoryUsage = 0;

        if (processHandle) {
            PROCESS_MEMORY_COUNTERS memoryCounters;
            if (GetProcessMemoryInfo(processHandle, &memoryCounters, sizeof(memoryCounters))) {
                memoryUsage = memoryCounters.WorkingSetSize;
            }
            CloseHandle(processHandle);
        }

        localArray = realloc(localArray, sizeof(ProcessInfo) * (count + 1));
        localArray[count].pid = processEntry.th32ProcessID;
        strncpy(localArray[count].processName, processEntry.szExeFile, MAX_PATH);
        localArray[count].memoryUsage = memoryUsage;
        count++;

    } while (Process32Next(snapshotHandle, &processEntry));

    CloseHandle(snapshotHandle);

    *processArray = localArray;
    *arrayCount = count;

    return 1;
}

void FreeProcessList(ProcessInfo* processArray, int arrayCount) {
    free(processArray);
}

int TerminateProcessByPID(DWORD targetPID) {
    HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, FALSE, targetPID);
    if (!processHandle) {
        return 0;
    }
    int result = TerminateProcess(processHandle, 0);
    CloseHandle(processHandle);
    return result;
}

int ChangeProcessPriority(DWORD targetPID, DWORD priorityLevel) {
    HANDLE processHandle = OpenProcess(PROCESS_SET_INFORMATION, FALSE, targetPID);
    if (!processHandle) {
        return 0;
    }
    int result = SetPriorityClass(processHandle, priorityLevel);
    CloseHandle(processHandle);
    return result;
}

int CompareByPID(const void* first, const void* second) {
    ProcessInfo* process1 = (ProcessInfo*)first;
    ProcessInfo* process2 = (ProcessInfo*)second;
    return (int)(process1->pid - process2->pid);
}

int CompareByMemory(const void* first, const void* second) {
    ProcessInfo* process1 = (ProcessInfo*)first;
    ProcessInfo* process2 = (ProcessInfo*)second;
    if (process1->memoryUsage > process2->memoryUsage) return -1;
    if (process1->memoryUsage < process2->memoryUsage) return 1;
    return 0;
}

int CompareByName(const void* first, const void* second) {
    ProcessInfo* process1 = (ProcessInfo*)first;
    ProcessInfo* process2 = (ProcessInfo*)second;
    return strcmp(process1->processName, process2->processName);
}
