#ifndef EMPLOYEES_H
#define EMPLOYEES_H

struct Employee {
    char firstName[20];
    char lastName[20];
    int age;
    char department[20];
};

void addEmployee();

void displayEmployees();

#endif // EMPLOYEES_H
