#include <windows.h>
#include <iostream>
#include "Employee.h"

int main(int argc, char *argv[]) {

    HANDLE orderPipe = CreateFile(
            argv[0], GENERIC_READ, FILE_SHARE_WRITE, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE requestPipe = CreateFile(
            argv[1], GENERIC_WRITE, FILE_SHARE_READ, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    bool terminate = false;
    while (!terminate) {
        DWORD bytesWrite;
        DWORD bytesRead;
        std::cout << "Введите цифру в соответствии с желаемым действием:\n"
                     "0 - Модифицировать запись.\n"
                     "1 - Прочитать запись.\n"
                     "2 - Завершить работу.\n";
        short action;
        std::cin >> action;
        if (action == 0) {
            std::cout << "Введите ID сотрудника, чья запись будет модифицирована:";
            int ID;
            std::cin >> ID;

            request tempRequest = {ID, request::OVERWRITE};
            WriteFile(requestPipe, &tempRequest, sizeof(request), &bytesWrite, NULL);

            order tempOrder = {};
            ReadFile(orderPipe, &tempOrder, sizeof(order), &bytesRead, NULL);

            employee temp = {};
            std::cout << "Введите запись (в виде номер сотрудника, имя сотрудника, отработанные часы):\n";
            std::cin >> temp.ID >> temp.fullName >> temp.hoursWorked;
        } else if (action == 1) {
            std::cout << "Введите ID сотрудника, чья запись будет прочитана:";
            int ID;
            std::cin >> ID;

            request tempRequest = {ID, request::READ};
            WriteFile(requestPipe, &tempRequest, sizeof(request), &bytesWrite, NULL);

            order tempOrder = {};
            ReadFile(orderPipe, &tempOrder, sizeof(order), &bytesRead, NULL);

            std::cout << tempOrder.record.ID << " " << tempOrder.record.fullName << " " << tempOrder.record.hoursWorked << ";\n";
        } else if (action == 2) terminate = true;
        else std::cout << "Введен неверный код действия.\n";
    }

    CloseHandle(orderPipe);
    CloseHandle(requestPipe);
    return 0;
}