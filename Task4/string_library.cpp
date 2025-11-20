#include "string_library.h"
#include <iostream>
using namespace std;

int str_len(char str[]) {
    int length = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        length++;
    }
    return length;
}


void to_upper(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - 32;
        }
    }
}


void to_lower(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + 32;
        }
    }
}

int str_cmp(char str1[], char str2[]) {
    // Check lengths first
    if (str_len(str1) > str_len(str2))
    {
        return 5;
    }
    else if (str_len(str1) < str_len(str2))
    {
        return -5;
    }
    

    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            if (str1[i] < str2[i]) {
                return -1;
            } else {
                return 1;
            }
        }
        i++;
    }
    return 0;
}


void str_copy(char str1[], char str2[]) {
    int i = 0;
    while (str2[i] != '\0') {
        str1[i] = str2[i];
        i++;
    }
    str1[i] = '\0';
}

void str_concat(int str1_size, char str1[], char str2[]) {
    if (str1_size < str_len(str1) + str_len(str2) + 1) {
        cout << "Destination string is not large enough" << endl;
        return;
    }
    int i = str_len(str1);
    for (int j = 0; j <= str_len(str2); j++) {
        str1[i + j] = str2[j];
    }

}
