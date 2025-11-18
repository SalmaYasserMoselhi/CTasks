#include <iostream>
#include "string_library.h"
using namespace std;
int main() {
    char str1[] = "Hello\0";
    int length = str_len(str1);
    cout << "Length of '" << str1 << "' is: " << length << endl;
    
    // cout<< (int)'a' << " " << (int)'A' << endl;
    to_upper(str1);
    cout << "Uppercase: " << str1 << endl;

    to_lower(str1);
    cout << "Lowercase: " << str1 << endl;

    char str2[] = "hello\0";
    char str3[] = "Hello\0";
    int cmpResult1 = str_cmp(str1, str2);
    int cmpResult2 = str_cmp(str1, str3);
    cout << "Comparison of '" << str1 << "' and '" << str2 << "': " << cmpResult1 << endl;
    cout << "Comparison of '" << str1 << "' and '" << str3 << "': " << cmpResult2 << endl;
    return 0;
}