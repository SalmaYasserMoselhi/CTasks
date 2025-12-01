#include "list.h"
#include <iostream>
using namespace std;

int main() {
    // Demo list
    List l1;
    l1.append(1);
    l1.append(2.5f);
    l1.append('-');
    l1.append(-10.5f);
    l1.append('a');
    l1.append("hello");
    l1.append(42);

    List* nested = new List();
    nested->append(90);
    nested->append('A');

    l1.append(nested);
    l1.append("Loma");
    
    cout << "l1 = ";
    l1.display();
    cout << endl;
    
    // User input list
    List l;
    int size;
    string input;
    
    cout << "\n========== Create Your List ==========" << endl;
    cout << "Enter list size: ";
    cin >> size;
    cin.ignore(); 
    
    // Validate size
    if(size < 0) {
        cout << "Invalid size!" << endl;
        size = 0;
    }
    
    // Get elements
    for(int i = 0; i < size; i++) {
        cout << "Enter element " << (i + 1) << ": ";
        getline(cin, input);
        l.append(input);
    }
    
    cout << "\n========== Your List ==========" << endl;
    l.display();
    cout << "\nTotal elements: " << l.length() << endl;
    
    return 0;
}