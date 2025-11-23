#ifndef EMPLOYEES_H
#define EMPLOYEES_H
#include <string>
using namespace std;

struct Employee {
    string firstName;
    string lastName;
    int age;
    string department;
};

void addEmployee();

void displayEmployees();

#endif // EMPLOYEES_H
