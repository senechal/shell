#ifndef PATH_H
#define PATH_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mystring.h>
#include<string_list.h>

#define MAX_SIZE_IN 255
#define ABSOLUTE 1
#define RELATIVE 0

String get_path(char * env[]);

StringList get_path_array(char * env[]);

StringList path_concat(char * env[], String command);

int is_absolute(String path);

#endif
