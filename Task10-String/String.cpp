#include "String.h"

// Helper Functions
int String::length(const char* str) {
    int count = 0;
    while (str[count] != '\0') count++;
    return count;
}

char* String::strcpy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

char* String::strcat(char* dest, const char* src) {
    int i = 0;
    while (dest[i] != '\0') i++;
    int j = 0;
    while (src[j] != '\0') {
        dest[i++] = src[j++];
    }
    dest[i] = '\0';
    return dest;
}

// Constructors
String::String() {
    len = 0;
    data = new char[1];
    data[0] = '\0';
}


String::String(const char* str) {
    if (!str) {
        len = 0;
        data = new char[1];
        data[0] = '\0';
    } else {
        len = length(str);
        data = new char[len + 1];
        strcpy(data, str);
    }
}

// Deep copy
String::String(const String& other) {
    len = other.len;
    data = new char[len + 1];
    strcpy(data, other.data);
}

// Destructor
String::~String() {
    delete[] data;
}


// Assignment Operator
String& String::operator=(const String& other) {
    if (this != &other) {
        delete[] data;
        len = other.len;
        data = new char[len + 1];
        strcpy(data, other.data);
    }
    return *this;
}

int String::length() const {
    return len;
}

bool String::isEmpty() const {
    return len == 0;
}

// Access Elements by index
char& String::operator[](int index) {
    return data[index];
}


void String::append(const String& str) {
    char* temp = new char[len + str.len + 1];
    strcpy(temp, data);
    strcat(temp, str.data);
    delete[] data;
    data = temp;
    len += str.len;
}

void String::append(const char* str) {
    if (str) {
        int strLen = length(str);
        char* temp = new char[len + strLen + 1];
        strcpy(temp, data);
        strcat(temp, str);
        delete[] data;
        data = temp;
        len += strLen;
    }
}

void String::append(char ch) {
    char* temp = new char[len + 2];
    strcpy(temp, data);
    temp[len] = ch;
    temp[len + 1] = '\0';
    delete[] data;
    data = temp;
    len++;
}

// Compare Operator
bool String::operator==(const String& other) const {
    int i = 0;
    while (data[i] != '\0' && other.data[i] != '\0') {
        if (data[i] != other.data[i])
            return false;
        i++;
    }
    return data[i] == other.data[i];
}

bool String::operator!=(const String& other) const {
    return !(*this == other);
}

String String::operator+(const String& other) const {
    String result;
    delete[] result.data;
    result.len = len + other.len;
    result.data = new char[result.len + 1];
    strcpy(result.data, data);
    strcat(result.data, other.data);
    return result;
}

String String::operator+(const char* str) const {
    String result;
    delete[] result.data;
    int strLen = str ? length(str) : 0;
    result.len = len + strLen;
    result.data = new char[result.len + 1];
    strcpy(result.data, data);
    if (str) strcat(result.data, str);
    return result;
}

String String::operator+(char ch) const {
    String result;
    delete[] result.data;
    result.len = len + 1;
    result.data = new char[result.len + 1];
    strcpy(result.data, data);
    result.data[len] = ch;
    result.data[len + 1] = '\0';
    return result;
}

String& String::operator+=(const String& other) {
    append(other);
    return *this;
}

String& String::operator+=(const char* str) {
    append(str);
    return *this;
}

String& String::operator+=(char ch) {
    append(ch);
    return *this;
}

// os operator
std::ostream& operator<<(std::ostream& os, const String& str) {
    os << str.data;
    return os;
}

String String::toUpper() const {
    String result(*this);
    for (int i = 0; i < len; i++) {
        if (result.data[i] >= 'a' && result.data[i] <= 'z')
            result.data[i] -= ('a' - 'A');
    }
    return result;
}

String String::toLower() const {
    String result(*this);
    for (int i = 0; i < len; i++) {
        if (result.data[i] >= 'A' && result.data[i] <= 'Z')
            result.data[i] += ('a' - 'A');
    }
    return result;
}

void String::reverse() {
    for (int i = 0; i < len / 2; i++) {
        char temp = data[i];
        data[i] = data[len - 1 - i];
        data[len - 1 - i] = temp;
    }
}