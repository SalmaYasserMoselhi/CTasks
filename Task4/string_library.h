#ifndef STRING_LIBRARY_H
#define STRING_LIBRARY_H

int str_len(char str[]);
int str_cmp(char str1[], char str2[]);
void to_upper(char str[]);
void to_lower(char str[]);
void str_copy(char dest[], char src[]);
void str_concat(char dest[], char src[]);

#endif