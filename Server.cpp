#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include "Employee.h"

bool startProcess(const char *appName, const char *cmdLine) {
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    PROCESS_INFORMATION pi;

    if (!CreateProcess(appName, const_cast<char *>(cmdLine), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si,
                       &pi))
        return false;

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return true;
}

void printList(const std::string &listName) {
    std::ifstream list(listName.c_str(), std::ios::binary);
    employee temp = {};
    while (list.read((char *) &temp, sizeof(employee))) {
        std::cout << temp.ID << " " << temp.fullName << " " << temp.hoursWorked << ";\n";
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "Данная программа позволяет запись и чтение сообщений в/из бинарного файла различными потоками.\n";

    std::cout << "Введите имя бинарного файла для списка сотрудников (в виде \"имя_файла.bin\"):";
    std::string listName;
    std::cin >> listName;

    std::cout << "Введите количество записей в списке соответствующее количеству сотрудников:";
    int recordCount;
    std::cin >> recordCount;

    std::ofstream list(listName.c_str(), std::ios::binary);

    int i = 0;
    while (i < recordCount) {
        employee temp = {};
        std::cout << "Введите запись (в виде номер сотрудника, имя сотрудника, отработанные часы):\n";
        std::cin >> temp.ID >> temp.fullName >> temp.hoursWorked;
        list.write((char *) &temp, sizeof(employee));
        ++i;
    }

    printList(listName);

    std::cout << "Введите количество экземпляров процесса Client, которые будут запущены:";
    int processCount;
    std::cin >> processCount;

    i = 0;
    while (i < processCount) {
        HANDLE hNamedPipe = CreateNamedPipe("\\\\.\\pipe\\named_pipe", PIPE_ACCESS_DUPLEX,
                                            PIPE_TYPE_MESSAGE | PIPE_WAIT, processCount,
                                            0, 0, INFINITE, NULL);
        startProcess("client.exe", "");
        ConnectNamedPipe(hNamedPipe, NULL);
        ++i;
    }

    system("pause");
}