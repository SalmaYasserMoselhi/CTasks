#include "Stack.h"
#include <string>

int main() {
    // Test with integers
    cout << "=== Testing Stack<int> ===" << endl;
    Stack<int> s(5);
    
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
    Stack<int> s2(s);
    s2.display();
    
    cout << "\nTesting assignment operator:" << endl;
    Stack<int> s3(4);
    Stack<int> s4(3);
    s4 = s3 = s; 
    s3.display();
    s4.display();
    
    cout << "\nTesting Deep Copy:" << endl;
    s.pop();
    cout << "s1: ";
    s.display();
    cout << "s3: ";
    s3.display();
    
    // Test with doubles
    cout << "\n=== Testing Stack<double> ===" << endl;
    Stack<double> ds(3);
    ds.push(3.14);
    ds.push(2.71);
    ds.push(1.41);
    ds.display();
    cout << "Top: " << ds.top() << endl;
    
    // Test with strings
    cout << "\n=== Testing Stack<string> ===" << endl;
    Stack<string> ss(3);
    ss.push("Hello");
    ss.push("World");
    ss.push("!");
    ss.display();
    cout << "Popped: " << ss.pop() << endl;
    ss.display();
    
    // Test with characters
    cout << "\n=== Testing Stack<char> ===" << endl;
    Stack<char> cs(4);
    cs.push('A');
    cs.push('B');
    cs.push('C');
    cs.display();
    
    return 0;
}