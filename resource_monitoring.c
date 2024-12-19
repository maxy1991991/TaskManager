#include "user_interface.h"
#include "process_management.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static ProcessInfo* processListArray = NULL;
static int totalProcesses = 0;

static int (*sortFunction)(const void*, const void*) = CompareByPID;

void DisplayProcessList() {
    if (processListArray) {
        FreeProcessList(processListArray, totalProcesses);
        processListArray = NULL;
        totalProcesses = 0;
    }

    if (GetProcessList(&processListArray, &totalProcesses)) {
        qsort(processListArray, totalProcesses, sizeof(ProcessInfo), sortFunction);

        printf("PID\tMemory(MB)\tProcess Name\n");
        for (int i = 0; i < totalProcesses; i++) {
            printf("%u\t%.2f\t\t%s\n",
                processListArray[i].pid,
                processListArray[i].memoryUsage / (1024.0 * 1024.0),
                processListArray[i].processName);
        }
    }
    else {
        printf("Failed to retrieve process list.\n");
    }
}

int ProcessUserInput() {
    char userCommand[256];
    printf("\nEnter command (list [pid/mem/name], kill <PID>, priority <PID> <level>, quit): ");
    fgets(userCommand, sizeof(userCommand), stdin);

    char* inputToken = strtok(userCommand, " \n");
    if (inputToken == NULL) return 1;

    if (strcmp(inputToken, "list") == 0) {
        char* sortOption = strtok(NULL, " \n");
        if (sortOption) {
            if (strcmp(sortOption, "pid") == 0) {
                sortFunction = CompareByPID;
            } else if (strcmp(sortOption, "mem") == 0) {
                sortFunction = CompareByMemory;
            } else if (strcmp(sortOption, "name") == 0) {
                sortFunction = CompareByName;
            } else {
                printf("Invalid sorting parameter. Using default (PID).\n");
                sortFunction = CompareByPID;
            }
        }
        return 1;
    } else if (strcmp(inputToken, "kill") == 0) {
        char* pidString = strtok(NULL, " \n");
        if (pidString) {
            DWORD targetPID = atoi(pidString);
            if (TerminateProcessByPID(targetPID)) {
                printf("Process %u terminated successfully.\n", targetPID);
            } else {
                printf("Failed to terminate process %u.\n", targetPID);
            }
        } else {
            printf("Please specify a PID.\n");
        }
    } else if (strcmp(inputToken, "priority") == 0) {
        char* pidString = strtok(NULL, " \n");
        if (pidString) {
            DWORD targetPID = atoi(pidString);
            char* priorityLevel = strtok(NULL, " \n");
            if (priorityLevel) {
                DWORD priorityClass;
                if (strcmp(priorityLevel, "low") == 0) {
                    priorityClass = IDLE_PRIORITY_CLASS;
                } else if (strcmp(priorityLevel, "normal") == 0) {
                    priorityClass = NORMAL_PRIORITY_CLASS;
                } else if (strcmp(priorityLevel, "high") == 0) {
                    priorityClass = HIGH_PRIORITY_CLASS;
                } else if (strcmp(priorityLevel, "realtime") == 0) {
                    priorityClass = REALTIME_PRIORITY_CLASS;
                } else {
                    printf("Invalid priority level.\n");
                    return 1;
                }
                if (ChangeProcessPriority(targetPID, priorityClass)) {
                    printf("Priority of process %u changed successfully.\n", targetPID);
                } else {
                    printf("Failed to change priority of process %u.\n", targetPID);
                }
            } else {
                printf("Please specify a priority level (low, normal, high, realtime).\n");
            }
        } else {
            printf("Please specify a PID.\n");
        }
    } else if (strcmp(inputToken, "quit") == 0) {
        return 0;
    } else {
        printf("Invalid command.\n");
    }

    printf("Press Enter to continue...");
    getchar();
    return 1;
}
