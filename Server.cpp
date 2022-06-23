#include <iostream>
#include <fstream>
#include <Windows.h>
#include <sstream>
#include "Employee.h"

HANDLE startProcess(const char *appName, const char *cmdLine) {
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    PROCESS_INFORMATION pi;

    if (!CreateProcess(appName, const_cast<char *>(cmdLine), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si,
                       &pi))
        return NULL;

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    return pi.hThread;
}

DWORD WINAPI session(LPVOID lpParam) {
    int pipeIndex = (long long) lpParam;
    std::ostringstream tempStrStream;
    tempStrStream << pipeIndex;

    HANDLE orderPipe;
    std::string orderPipeName = "\\\\.\\pipe\\orderPipe" + tempStrStream.str();
    orderPipe = CreateNamedPipe(orderPipeName.c_str(), PIPE_ACCESS_OUTBOUND, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, INFINITE,
                                NULL);

    HANDLE requestPipe;
    std::string requestPipeName = "\\\\.\\pipe\\requestPipe" + tempStrStream.str();
    requestPipe = CreateNamedPipe(requestPipeName.c_str(), PIPE_ACCESS_INBOUND, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, INFINITE,
                                  NULL);

    std::string cmdLine = orderPipeName + " " + requestPipeName;
    startProcess("client.exe", cmdLine.c_str());

    bool terminate = false;
    while (!terminate) {
        DWORD bytesRead;
        DWORD bytesWrite;

        request clientRequest = {};
        ReadFile(requestPipe, &clientRequest, sizeof(request), &bytesRead, NULL);

        if (clientRequest.requestID == request::READ) {
            order serverOrder = {clientRequest.ID, order::ACCESS_ALLOWED, };
            WriteFile(orderPipe, &serverOrder, sizeof(order), &bytesWrite, NULL);
        } else if (clientRequest.requestID == request::OVERWRITE) {
            order serverOrder = {clientRequest.ID, order::ACCESS_ALLOWED, };
            WriteFile(orderPipe, &serverOrder, sizeof(serverOrder), &bytesWrite, NULL);
        } else terminate = true;
    }
    return 0;
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

    HANDLE *sessionThreads = new HANDLE[processCount];
    DWORD *ID = new DWORD[processCount];

    i = 0;
    while (i < processCount) {
        sessionThreads[i] = CreateThread(NULL, 0, session, (LPVOID) i, 0, &ID[i]);
        ++i;
    }

    WaitForMultipleObjects(processCount, sessionThreads, TRUE, INFINITE);

    printList(listName);

    system("pause");
    return 0;
}