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

            employee temp = {};

            request tempRequest = {ID, request::OVERWRITE, temp};
            DWORD bytesWrite;
            WriteFile(requestPipe, &tempRequest, sizeof(request), &bytesWrite, NULL);

            order tempOrder = {};
            DWORD bytesRead;
            ReadFile(orderPipe, &tempOrder, sizeof(order), &bytesRead, NULL);

            if (tempOrder.orderID == order::ACCESS_GRANTED) {
                std::cout << "Введите запись (в виде номер сотрудника, имя сотрудника, отработанные часы):\n";
                std::cin >> temp.ID >> temp.fullName >> temp.hoursWorked;
                tempRequest.record = temp;

                WriteFile(requestPipe, &tempRequest, sizeof(request), &bytesWrite, NULL);
                std::cout << "Введите любой символ для окончания доступа к записи:";
                char closeAccess;
                std::cin >> closeAccess;
                tempRequest.requestID = request::CLOSE_ACCESS;
                WriteFile(requestPipe, &tempRequest, sizeof(request), &bytesWrite, NULL);
            } else if (tempOrder.orderID == order::ACCESS_DENIED) {
                std::cout << "Доступ к модификации записи №" << ID
                          << " заблокирован, так как она используется другим клиентом\n";
            }
        } else if (action == 1) {
            std::cout << "Введите ID сотрудника, чья запись будет прочитана:";
            int ID;
            std::cin >> ID;

            employee temp = {};

            request tempRequest = {ID, request::READ, temp};
            DWORD bytesWrite;
            WriteFile(requestPipe, &tempRequest, sizeof(request), &bytesWrite, NULL);

            order tempOrder = {};
            DWORD bytesRead;
            ReadFile(orderPipe, &tempOrder, sizeof(order), &bytesRead, NULL);
            if (tempOrder.orderID == order::ACCESS_GRANTED) {
                std::cout << tempOrder.record.ID << " " << tempOrder.record.fullName << " "
                          << tempOrder.record.hoursWorked
                          << ";\n";
                std::cout << "Введите любой символ для окончания доступа к записи:";
                char closeAccess;
                std::cin >> closeAccess;
                tempRequest.requestID = request::CLOSE_ACCESS;
                WriteFile(requestPipe, &tempRequest, sizeof(request), &bytesWrite, NULL);
            } else if (tempOrder.orderID == order::ACCESS_DENIED) {
                std::cout << "Доступ к чтению записи №" << ID
                          << " заблокирован, так как она используется другим клиентом\n";
            }
        } else if (action == 2) {
            employee temp = {};

            request tempRequest = {0, request::TERMINATE, temp};

            DWORD bytesWrite;
            WriteFile(requestPipe, &tempRequest, sizeof(request), &bytesWrite, NULL);
            terminate = true;
        } else std::cout << "Введен неверный код действия.\n";
    }

    CloseHandle(orderPipe);
    CloseHandle(requestPipe);
    return 0;
}