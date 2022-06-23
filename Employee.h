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
        READ, OVERWRITE, TERMINATE
    } requestID;
};
struct order {
    int ID;
    enum orderID {
        ACCESS_BLOCKED, ACCESS_ALLOWED
    } orderID;
    employee record;
};
#endif