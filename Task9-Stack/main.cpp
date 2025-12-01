#include "stack.h"

int main() {
    Stack s(5);
    
    s.push(10);
    s.push(20);
    s.push(30);
    
    cout << "After pushing 10, 20, 30:" << endl;
    s.display();
    cout << "Size: " << s.size() << endl;
    
    cout << "\nTop element: " << s.top() << endl;
    
    cout << "\nPop: " << s.pop() << endl;
    s.display();
    
    s.push(40);
    s.push(50);
    s.push(60);
    
    cout << "\nAfter more pushes:" << endl;
    s.display();
    
    cout << "\nTesting copy constructor:" << endl;
    Stack s2 = s; // or Stack s2(s) --> Both calls deep copy constructor
    s2.display();
    
    cout << "\nTesting assignment operator:" << endl;
    Stack s3(4);
    Stack s4(3);
    s4 = s3 = s; 
    s3.display();
    s4.display();
    
    cout << "\nTesting Deep Copy:" << endl;
    s.pop();
    cout<<"s1: ";
    s.display();
    cout<<"s3: ";
    s3.display();
    return 0;
}