#include "employees.h"
#include <iostream>
using namespace std;

#define MAX_EMPLOYEES 100

static Employee employees[MAX_EMPLOYEES];
static int employeeCount = 0;

void addEmployee() {
    if (employeeCount >= MAX_EMPLOYEES) {
        cout << "Cannot add more employees.\n";
        return;
    }

    cout << "Enter firstName: ";
    cin >> employees[employeeCount].firstName;

    cout << "Enter lastName: ";
    cin >> employees[employeeCount].lastName;

    cout << "Enter age: ";
    cin >> employees[employeeCount].age;

    cout << "Enter department: ";
    cin >> employees[employeeCount].department;

    employeeCount++;
    cout << "Employee added successfully.\n";
}

void displayEmployees() {
    if (employeeCount == 0) {
        cout << "No employees to display.\n";
        return;
    }

    for (int i = 0; i < employeeCount; i++) {
        cout << "Employee [" << i << "]\n";
        cout << "First Name: " << employees[i].firstName << "\n";
        cout << "Last Name: " << employees[i].lastName << "\n";
        cout << "Age: " << employees[i].age << "\n";
        cout << "Department: " << employees[i].department << "\n";
        cout << "-----------------------------\n";
    }
}
