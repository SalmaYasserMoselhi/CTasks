#include "employees.h"
#include <iostream>
#include <limits>
#include <cctype>
using namespace std;

#define MAX_EMPLOYEES 100

static Employee employees[MAX_EMPLOYEES];
static int employeeCount = 0;

// Helper: check string is alphabetic
bool isAlphaString(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isalpha(c)) return false;
    }
    return true;
}

// Helper: read validated integer
int readInt(const string& msg, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << msg;
        if (cin >> value) {
            if (value >= minVal && value <= maxVal) {
                return value;
            } else {
                cout << "❌ Invalid range! Enter a value between "
                     << minVal << " and " << maxVal << ".\n";
            }
        } else {
            cout << "❌ Invalid number! Try again.\n";
        }

        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    }
}

// Helper: read validated alphabetic string
string readAlpha(const string& msg) {
    string s;
    while (true) {
        cout << msg;
        cin >> s;
        if (isAlphaString(s)) return s;
        cout << "❌ Invalid text! Only alphabetic letters allowed.\n";
    }
}

void addEmployee() {
    if (employeeCount >= MAX_EMPLOYEES) {
        cout << "Cannot add more employees.\n";
        return;
    }

    employees[employeeCount].firstName  = readAlpha("Enter firstName: ");
    employees[employeeCount].lastName   = readAlpha("Enter lastName: ");
    employees[employeeCount].age        = readInt("Enter age (18–70): ", 18, 70);
    employees[employeeCount].department = readAlpha("Enter department: ");

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
