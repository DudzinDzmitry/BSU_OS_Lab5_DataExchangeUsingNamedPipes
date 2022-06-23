#ifndef LAB5_EMPLOYEE_H
#define LAB5_EMPLOYEE_H
struct employee {
    int ID;
    char fullName[10];
    double hoursWorked;
};
struct request {
    int ID;
    enum requestID {
        OVERWRITE, READ, CLOSE_ACCESS, TERMINATE
    } requestID;
    employee record;
};

struct order {
    enum orderID {
        ACCESS_DENIED, ACCESS_GRANTED
    } orderID;
    employee record;
};

#endif