#include "complex.h"

int main() {
    Complex c1;             // default constructor
    Complex c2(5, 3);       // parameterized
    Complex c3(10);         // only real part
    
    cout << "c1 = " << c1 << endl;
    cout << "c2 = " << c2 << endl;
    cout << "c3 = " << c3 << endl;
    cout << "---------------------------------\n";
    
    c1.setReal(7);
    c1.setImag(9);
    cout << "c1 = " << c1.getReal() << " + " << c1.getImag() << "i" << endl;
    cout << "---------------------------------\n";
    
    cout << "Display c2: ";
    c2.display(); // Display using member function
    cout << endl;
    cout << "---------------------------------\n";
    
    Complex sum1 = c2 + c3; // Complex + Complex
    cout << c2 << " + " << c3 << " = " << sum1 << endl;
    cout << "---------------------------------\n";
    
    Complex sum2 = c2 + 10; // Complex + value
    cout << c2 << " + 10 = " << sum2 << endl;
    cout << "---------------------------------\n";
    
    Complex sum3 = 20 + c2; // value + Complex
    cout << "20 + " << c2 << " = " << sum3 << endl;
    cout << "---------------------------------\n";
    
    Complex diff1 = c2 - c3; // Complex - Complex
    cout << c2 << " - " << c3 << " = " << diff1 << endl;
    cout << "---------------------------------\n";
    
    Complex diff2 = c2 - 4; // Complex - value
    cout << c2 << " - 4 = " << diff2 << endl;
    cout << "---------------------------------\n";
    
    Complex mul = c2 * c3; // Complex * Complex
    cout << c2 << " * " << c3 << " = " << mul << endl;
    cout << "---------------------------------\n";
    
    cout << "-" << c2 << " = " << -c2 << endl;
    cout << "---------------------------------\n";
    
    Complex div = c3 / c2; // Complex / Complex
    cout << c3 << " / " << c2 << " = " << div << endl;
    cout << "---------------------------------\n";
    
    // denom = 0 check
    Complex zero(0, 0);
    try {
        Complex divByZero = c3 / zero; // This should throw an exception
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }
    cout << "---------------------------------\n";
    
    Complex c4(1, 1);
    cout << "c4 = " << c4 << endl;
    c4 += c2; // c2 = 5 + 3i, c4 = 1 + 1i
    cout << "c4 += c2 -> c4 = " << c4 << endl; // c4 = 6 + 4i
    c4 -= c3; // c3 = 10 + 0i
    cout << "c4 -= c3 -> c4 = " << c4 << endl; // c4 = -4 + 4i
    cout << "---------------------------------\n";
    
    Complex c5;
    c5 = c2;
    cout << "c5 = " << c5 << endl;
    cout << "---------------------------------\n";
    
    cout << c2 << " == " << c5 << " ? " << (c2 == c5 ? "true" : "false") << endl;
    cout << c2 << " != " << c3 << " ? " << (c2 != c3 ? "true" : "false") << endl;
    cout << "---------------------------------\n";

    // Test subscript operator []
    Complex c6(8, 5);
    cout << "c6 = " << c6 << endl;
    cout << "c6[0] (real part) = " << c6[0] << endl;
    cout << "c6[1] (imaginary part) = " << c6[1] << endl;
    c6[0] = 15;
    c6[1] = 20;
    cout << "After c6[0] = 15 and c6[1] = 20: c6 = " << c6 << endl;
    try {
        cout << c6[2] << endl;
    } catch (const out_of_range& e) {
        cout << e.what() << endl;
    }
    cout << "---------------------------------\n";
    
    // Test prefix increment (++c)
    Complex c7(3, 4);
    cout << "c7 = " << c7 << endl;
    ++c7;
    cout << "++c7 = " << c7 << endl;
    cout << "---------------------------------\n";
    
    // Test postfix increment (c++)
    Complex c8(7, 2);
    cout << "c8 = " << c8 << endl;
    Complex c8_copy = c8++;
    cout << "c8++ returns " << c8_copy << endl;
    cout << "After c8++, c8 = " << c8 << endl;
    cout << "---------------------------------\n";
    
    // Test logical NOT operator (!)
    Complex c9(0, 0);
    Complex c10(5, 3);
    cout << "c9 = " << c9 << endl;
    cout << "c10 = " << c10 << endl;
    cout << "!c9 (is zero?) = " << (!c9 ? "true" : "false") << endl;
    cout << "!c10 (is zero?) = " << (!c10 ? "true" : "false") << endl;
    cout << "---------------------------------\n";

    Complex c11(5,10);
    cout << "Magnitute : " << c11() << endl;
    
    Complex userInput;
    cin >> userInput;
    cout << "You entered: " << userInput << endl;
    
    return 0;
}