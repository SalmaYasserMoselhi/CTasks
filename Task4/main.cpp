#include <iostream>
#include "string_library.h"
using namespace std;
int main() {
    char str1[] = "Hello\0";
    int length = str_len(str1);
    cout << "Length of '" << str1 << "' is: " << length << endl;
    cout << endl;
    
    // cout<< (int)'a' << " " << (int)'A' << endl;
    to_upper(str1);
    cout << "Uppercase: " << str1 << endl;

    to_lower(str1);
    cout << "Lowercase: " << str1 << endl;
    cout << endl;


    char str2[] = "hello\0";
    char str3[] = "Hello\0";
    char str4[] = "Hello World\0";
    cout << "Comparison of '" << str1 << "' and '" << str2 << "': " << str_cmp(str1, str2) << endl;
    cout << "Comparison of '" << str1 << "' and '" << str3 << "': " << str_cmp(str1, str3) << endl;
    cout << "Comparison of '" << str1 << "' and '" << str4 << "': " << str_cmp(str1, str4) << endl;
    cout << endl;


    char dest[] = "Test\0";
    char src[] = "Little Lomy\0";
    str_copy(dest, src);
    cout << "After copying, dest contains: '" << dest << "'" << endl;


    char dest2[5];
    char src2[] = "Another Try\0";
    str_copy(dest2, src2);
    cout << "After copying, dest2 contains: '" << dest2 << "'" << endl;
    cout << endl;

    char str_concat1[] = "Hello, \0";
    char str_concat2[] = "Lomy\0";
    str_concat(str_concat1, str_concat2);
    cout << "After concatenation: '" << str_concat1 << "'" << endl;

    return 0;
}