#ifndef MYSTRING_H
#define MYSTRING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef char * String;

char * copy_string(char * origin);

int size_of(char * str);

char * shift_string_right(char * str, int n);

int remove_char(String *string, int index);

int remove_last_char(String *string);

char * insert_string(char * str_origin, char * str_destine);

char * new_string();

char * new_string_size(int size);

void append(String * string, char c);

void append_string(String * string, char * app);

char * int_to_string(int integer);

int string_to_int(char * string);

char bin_to_char(int bin);

void insert_char (char origin, char ** destine);

int string_to_file(FILE *file, char * string);

int string_from_file(FILE *file, char ** string);

void dispose_string(String *string);

int find_char_at_last(String string, char element);

int find_char_at_pos(String string, char element, int index);

int is_empty_string(String string);

int string_check_index(String string, int index);

String copy_substring(String origin, int start, int end);

int equals(String str1, String str2);

#endif
