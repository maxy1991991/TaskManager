#ifndef PROCESS_MANAGEMENT_H
#define PROCESS_MANAGEMENT_H

#include <windows.h>

typedef struct {
    DWORD pid;
    char processName[MAX_PATH];
    SIZE_T memoryUsage;
} ProcessInfo;

int GetProcessList(ProcessInfo** processList, int* processCount);
void FreeProcessList(ProcessInfo* processList, int processCount);
int TerminateProcessByPID(DWORD pid);
int ChangeProcessPriority(DWORD pid, DWORD priorityClass);

int CompareByPID(const void* a, const void* b);
int CompareByMemory(const void* a, const void* b);
int CompareByName(const void* a, const void* b);

#endif // PROCESS_MANAGEMENT_H
