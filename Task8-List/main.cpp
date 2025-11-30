#include "list.h"
#include <iostream>
using namespace std;

int main() {
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
    cout<<endl;
    
    List l;
    string input;
    
    cout << "Enter elements to add to the list." << endl;
    cout << "Type 'done' when finished." << endl;
    
    while(true) {
        cout << "\nEnter element: ";
        getline(cin, input);
        
        if(input == "done") {
            break;
        }
        
        l.append(input);
    }
    
    cout << "\n========== Your List ==========" << endl;
    l.display();
    cout << "\nTotal elements: " << l.length() << endl;
    
    return 0;
}