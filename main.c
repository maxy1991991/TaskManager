#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "process_management.h"
#include "resource_monitoring.h"
#include "user_interface.h"

int main() {
    if (!InitializeSystemMonitoring()) {
        fprintf(stderr, "Error: Failed to initialize system monitoring.\n");
        return EXIT_FAILURE;
    }

    while (1) {
        system("cls");

        ShowSystemResources();
        ShowProcessList();

        if (!HandleUserInput()) {
            break;
        }

        Sleep(1000); 
    }

    CleanupSystemMonitoring();

    return EXIT_SUCCESS;
}
