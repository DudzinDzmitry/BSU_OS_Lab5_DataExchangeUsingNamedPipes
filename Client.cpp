#include <windows.h>
#include <iostream>
#include <string>
#include <codecvt>
#include <locale>
#include <conio.h>
#include <sstream>

int main() {
    SetConsoleOutputCP(CP_UTF8);

    WaitNamedPipe("\\\\.\\pipe\\named_pipe", INFINITE);
    HANDLE hPipe = CreateFile(
            "\\\\.\\pipe\\named_pipe", GENERIC_READ | GENERIC_WRITE, 0,
            NULL, OPEN_EXISTING, 0, NULL);

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
            int id;
            std::cin >> id;
            std::ostringstream tempStrStream;
            tempStrStream << id;
        } else if (action == 1) {
            std::cout << "Введите ID сотрудника, чья запись будет модифицирована:";
            int id;
            std::cin >> id;
            std::ostringstream tempStrStream;
            tempStrStream << id;
        } else if (action == 2) terminate = true;
        else std::cout << "Введен неверный код действия.\n";
    }
}