# Task Manager

A command-line utility developed in C/C++ that provides essential system monitoring and process management features for Windows. It enables users to monitor CPU and memory usage, as well as manage running processes, including modifying their priority or terminating them.

## Features

- Display a list of all active processes with details such as PID, memory consumption, and process name.
- Monitor real-time system resources, including CPU and memory usage.
- Terminate a process using its PID.
- Adjust the priority of running processes.
- Sort the process list by PID, memory usage, or name for better readability.

## Prerequisites

- **Windows 10 SDK** (for `pdh.h` and `psapi.h` libraries)
- **GCC** (via MinGW-w64) or **Microsoft Visual C++ Compiler** to compile the code

## Building and Running
1. **Install MinGW-w64** and the **Windows SDK**.
2. **Clone the repository** and navigate to the directory:
   ```bash
   git clone https://github.com/maxy1991991/TaskManager.git
   cd TaskManager
   ```
3. **Compiling**
   ```bash
   cl /FeTaskManager.exe src\main.c src\process_management.c src\resource_monitoring.c src\user_interface.c /link pdh.lib psapi.lib
    .\TaskManager.exe
   ```
   or
   ```bash
   gcc -o TaskManager.exe main.c process_management.c resource_monitoring.c user_interface.c -lpdh -lpsapi
    .\TaskManager.exe
   ```
   Note that you may need to use the `-L` and `-I` flags for the libraries
   
## Usage
After starting the application, the following commands can be used:
- `list [pid|mem|name]` - Displays the list of processes and sorts them by the selected criterion.
- `kill <PID>` - Terminates the process with the given PID.  
- `priority <PID> <level>` - Modifies the priority of the specified process. Valid priority levels include low, normal, high, and realtime.
