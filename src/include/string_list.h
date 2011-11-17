#ifndef STRING_LIST_H
#define STRING_LIST_H

#include "mystring.h"

typedef struct list{
    String * string;
    int size;
    int index;
}StringList;

// teste tad lista
StringList new_string_list();

void dispose_string_list(StringList *list);

//int add(StringList list, String element, int index);

int list_append(StringList *list, String element);

StringList explode(String string, char separator);

int list_find_char_at_last(StringList list, char element);

String list_current(StringList list);

String list_next(StringList * list);

int set_index(StringList * list, int index);

void reset_index(StringList * list);

String get_string(StringList list, int index);

void print_list(StringList list);

void trim_string_list(StringList * list, char element);

int is_valid_index(StringList list, int index);

int list_remove(StringList *list, int index);

int list_remove_current(StringList *list);

StringList from_array(char * array[]);

StringList from_array_size(char * array[], int size);

char ** to_array(StringList string_list);

int find_string(StringList list, String str); 

int * number_str(StringList list, String str); 

#endif

