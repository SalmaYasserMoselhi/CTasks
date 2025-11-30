#include "employees.h"
#include <iostream>
#include <limits>
#include <cctype>
using namespace std;

#define MAX_EMPLOYEES 100

static Employee employees[MAX_EMPLOYEES];
static int employeeCount = 0;

bool isAlphaString(const string& s) {
    for (unsigned char c : s) {
        if (isspace(c)) return false; 
        if (!isalpha(c)) return false;
    }
    return true;
}

bool hasSpaces(const string& str) {
    for (char c : str) {
        if (isspace(static_cast<unsigned char>(c))) {
            return true;
        }
    }
    return false;
}

int readInt(const string& msg, int minVal, int maxVal) {
    string input;
    int value;
    while (true) {
        cout << msg;
        if (!getline(cin, input)) return minVal;
        
        if (input.empty()) {
            cout << "Input cannot be empty. Try again.\n";
            continue;
        }
        
        if (hasSpaces(input)) {
            cout << "Invalid input! Spaces are not allowed.\n";
            continue;
        }
        
        try {
            value = stoi(input);
            if (value >= minVal && value <= maxVal) {
                return value;
            } else {
                cout << "Invalid range! Enter a value between "
                     << minVal << " and " << maxVal << ".\n";
            }
        } catch (const exception& e) {
            cout << "Invalid number! Try again.\n";
        }
    }
}

string readAlpha(const string& msg) {
    string s;
    while (true) {
        cout << msg;
        if (!std::getline(cin, s)) return string(); 
        if (s.empty()) {
            cout << "Input cannot be empty. Try again.\n";
            continue;
        }
        
        if (hasSpaces(s)) {
            cout << "Invalid input! Spaces are not allowed.\n";
            continue;
        }

        if (isAlphaString(s)) return s;
        cout << "Invalid text! Only alphabetic letters allowed (no spaces).\n";
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
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
