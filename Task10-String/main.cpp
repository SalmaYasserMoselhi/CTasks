#include <iostream>
#include "String.h"
using namespace std;

int main() {

    cout << "=== Testing Constructors ===\n";
    String s1;
    String s2("Hello");
    String s3(s2);

    cout << "s1 = \"" << s1 << "\"\n";
    cout << "s2 = \"" << s2 << "\"\n";
    cout << "s3 (copy of s2) = \"" << s3 << "\"\n\n";

    cout << "=== Testing Assignment ===\n";
    s1 = s2;
    cout << "s1 = \"" << s1 << "\"\n\n";

    cout << "=== Testing Append ===\n";
    s1.append(" World");
    cout << "s1.append(\" World\") -> " << s1 << "\n";

    s1.append('!');
    cout << "s1.append('!') -> " << s1 << "\n";

    String extra("+++");
    s1.append(extra);
    cout << "s1.append(extra) -> " << s1 << "\n\n";

    cout << "=== Testing Operators + and += ===\n";
    String a("Hi");
    String b(" there");

    String c = a + b;
    cout << "a + b = " << c << "\n";

    c = a + "!!!";
    cout << "a + \"!!!\" = " << c << "\n";

    c = a + '?';
    cout << "a + '?' = " << c << "\n";

    a += b;
    cout << "a += b -> " << a << "\n";

    a += "!!!";
    cout << "a += \"!!!\" -> " << a << "\n";

    a += '?';
    cout << "a += '?' -> " << a << "\n\n";

    cout << "=== Testing Comparisons ===\n";
    String x("Test");
    String y("Test");
    String z("Different");

    cout << "x == y ? " << (x == y) << "\n";
    cout << "x == z ? " << (x == z) << "\n";
    cout << "x != z ? " << (x != z) << "\n\n";

    cout << "=== Testing toUpper / toLower ===\n";
    String lower("abcDEF");
    cout << lower << " -> toUpper: " << lower.toUpper() << "\n";
    cout << lower << " -> toLower: " << lower.toLower() << "\n\n";

    cout << "=== Testing reverse ===\n";
    String r("ABCDE");
    cout << "Before: " << r << "\n";
    r.reverse();
    cout << "After : " << r << "\n\n";

    cout << "=== Testing isEmpty() and length() ===\n";
    cout << "s1 = " << s1 << "\n";
    cout << "length = " << s1.length() << "\n";
    cout << "isEmpty = " << s1.isEmpty() << "\n";

    cout << "\n=== Testing operator[] ===\n";
    cout << "s2 = " << s2 << "\n";
    cout << "s2[1] = " << s2[1] << "\n";
    s2[1] = 'A';
    cout << "after s2[1] = 'A' -> " << s2 << "\n";

    return 0;
}
