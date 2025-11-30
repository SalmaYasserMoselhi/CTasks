#include "datatype.h"

// Helper functions
bool isInteger(const string& s) {
    if(s.empty()) return false;
    size_t i = (s[0] == '+' || s[0] == '-') ? 1 : 0;
    if(i == s.size()) return false;
    for(; i < s.size(); ++i) {
        if(!isdigit(s[i])) return false;
    }
    return true;
}

bool isFloat(const string& s) {
    if(s.empty()) return false;
    bool dotSeen = false;
    size_t i = (s[0] == '+' || s[0] == '-') ? 1 : 0;
    if(i == s.size()) return false;
    for(; i < s.size(); ++i) {
        if(s[i] == '.') {
            if(dotSeen) return false;
            dotSeen = true;
        } else if(!isdigit(s[i])) {
            return false;
        }
    }
    return dotSeen;
}

DataType* getDataType(string input) {
    if(isInteger(input)) {
        return new Int(stoi(input));
    }
    if(isFloat(input)) {
        return new FLoat(stof(input));
    }
    if(input.length() == 1) {
        return new Char(input[0]);
    }
    return new String(input);
}

Int::Int(int n) {
    num = n;
}

FLoat::FLoat(float n) {
    num = n;
}

Char::Char(char c) {
    ch = c;
}

String::String(const char* s) {
    str = s;
}

String::String(string s) {
    str = s;
}

void Int::display() {
    cout << num;
}

void FLoat::display() {
    cout << num;
}

void Char::display() {
    cout << "'" << ch << "'";
}

void String::display() {
    cout << "\"" << str << "\"";
}
