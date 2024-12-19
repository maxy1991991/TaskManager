#include "user_interface.h"
#include "process_management.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static ProcessInfo* processList = NULL;
static int processCount = 0;

static int (*currentSortFunction)(const void*, const void*) = CompareByPID;


void DisplayProcessList() {
    if (processList) {
        FreeProcessList(processList, processCount);
        processList = NULL;
        processCount = 0;
    }

    if (GetProcessList(&processList, &processCount)) {
        qsort(processList, processCount, sizeof(ProcessInfo), currentSortFunction);

        printf("PID\tMemory(MB)\tProcess Name\n");
        for (int i = 0; i < processCount; i++) {
            printf("%u\t%.2f\t\t%s\n",
                processList[i].pid,
                processList[i].memoryUsage / (1024.0 * 1024.0),
                processList[i].processName);
        }
    }
    else {
        printf("Failed to retrieve process list.\n");
    }
}


int ProcessUserInput() {
    char command[256];
    printf("\nEnter command (list [pid/mem/name], kill <PID>, priority <PID> <level>, quit): ");
    fgets(command, sizeof(command), stdin);

    char* token = strtok(command, " \n");
    if (token == NULL) return 1;

    if (strcmp(token, "list") == 0) {
        // Check for sorting parameter
        token = strtok(NULL, " \n");
        if (token) {
            if (strcmp(token, "pid") == 0) {
                currentSortFunction = CompareByPID;
            }
            else if (strcmp(token, "mem") == 0) {
                currentSortFunction = CompareByMemory;
            }
            else if (strcmp(token, "name") == 0) {
                currentSortFunction = CompareByName;
            }
            else {
                printf("Invalid sorting parameter. Using default (PID).\n");
                currentSortFunction = CompareByPID;
            }
        }
        
        return 1;
    }
    else if (strcmp(token, "kill") == 0) {
        token = strtok(NULL, " \n");
        if (token) {
            DWORD pid = atoi(token);
            if (TerminateProcessByPID(pid)) {
                printf("Process %u terminated successfully.\n", pid);
            }
            else {
                printf("Failed to terminate process %u.\n", pid);
            }
        }
        else {
            printf("Please specify a PID.\n");
        }
    }
    else if (strcmp(token, "priority") == 0) {
        token = strtok(NULL, " \n");
        if (token) {
            DWORD pid = atoi(token);
            token = strtok(NULL, " \n");
            if (token) {
                DWORD priorityClass;
                if (strcmp(token, "low") == 0) {
                    priorityClass = IDLE_PRIORITY_CLASS;
                }
                else if (strcmp(token, "normal") == 0) {
                    priorityClass = NORMAL_PRIORITY_CLASS;
                }
                else if (strcmp(token, "high") == 0) {
                    priorityClass = HIGH_PRIORITY_CLASS;
                }
                else if (strcmp(token, "realtime") == 0) {
                    priorityClass = REALTIME_PRIORITY_CLASS;
                }
                else {
                    printf("Invalid priority level.\n");
                    return 1;
                }
                if (ChangeProcessPriority(pid, priorityClass)) {
                    printf("Priority of process %u changed successfully.\n", pid);
                }
                else {
                    printf("Failed to change priority of process %u.\n", pid);
                }
            }
            else {
                printf("Please specify a priority level (low, normal, high, realtime).\n");
            }
        }
        else {
            printf("Please specify a PID.\n");
        }
    }
    else if (strcmp(token, "quit") == 0) {
        return 0;
    }
    else {
        printf("Invalid command.\n");
    }

    printf("Press Enter to continue...");
    getchar();
    return 1;
}
