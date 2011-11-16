#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <mystring.h>

String copy_string(char * origin){
    int i = 0;
    char * destine = (char *) malloc(sizeof(char));
    while(origin[i]!='\0'){
        destine = (char*) realloc(destine, sizeof(char)*(i+2));
        destine[i] = origin[i];
        i++;
    }
    destine[i] = '\0';
    return destine;
}

int string_check_index(String string, int index){
    if(index < 0 || index >= size_of(string))
        return 0;
    else
        return 1;
}

String copy_substring(String origin, int start, int end){
    int i = start;
    String destine = new_string();
    if(!string_check_index(origin, start) || !string_check_index(origin, end))
        return new_string();
    while(i <= end){
        append(&destine, origin[i]);
        i++;
    }
    append(&destine, '\0');
    return destine;
}

int is_empty_string(String string){
    if(!string)
        return 0;
    if(size_of(string) == 0)
        return 1;
    else
        return 0;
}

int size_of(char * str){
    int n = 0;
    if(str == NULL)
        return -1;
    while(str[n] != '\0'){
        n++;
    }
    return n;
}

/*
 * Shifts the string to the right, allocating "n" more positions
 * on the begining of the string.
 */
char * shift_string_right(char * str, int n){
    int i = 0;
    int size = size_of(str);
    char * tmp = copy_string(str);
    tmp = (char *) realloc(tmp, sizeof(char)*(size+n+1));
    tmp[size+n] = '\0';
    for(i = size+n-1; i >= n; i--){
        tmp[i] = str[i-n];
    }
    for(i = 0; i < n; i++)
        tmp[i] = ' ';
    return tmp;
}

char * insert_string(char * str_origin, char * str_destine){
    int i;
    int size_orig = size_of(str_origin);
    //int size_dest = size_of(str_destine);
    char * str = shift_string_right(str_destine, size_orig);
    for(i = 0; i < size_orig; i++){
        str[i] = str_origin[i];
    }
    return str;
}

char * new_string(){
    char * string = (char *) malloc(sizeof(char));
    string[0] = '\0';
    return string;
}

char * new_string_size(int size){
    char * string = (char *) malloc(sizeof(char)*(size+1));
    int i;
    for(i = 0; i < size+1; i++){
        string[i] = ' ';
    }
    string[size] = '\0';
    return string;
}

void append(String * string, char c){
    int size = size_of(*string);
    *string = (String) realloc(*string, sizeof(char) * (size+2));
    (*string)[size] = c;
    (*string)[size+1] = '\0';
}

void append_string(String * string, char * app){
    int string_size = size_of((*string));
    int app_size = size_of(app);
    (*string) = (char *) realloc((*string), sizeof(char)*(app_size+string_size+1));
    int i;
    for(i = string_size; i < string_size + app_size; i++){
        (*string)[i] = app[i - string_size];
    }
    (*string)[app_size+string_size] = '\0';
}

char * int_to_string(int integer){
    char * buffer = new_string();
    sprintf(buffer, "%d", integer);
    return buffer;
}

int string_to_int(char * string){
    int i;
    int size = size_of(string);
    int n = 0;
    for(i = 0; i < size; i++){
        if(string[i] >= '0' && string[i] <= '9')
            n += pow(10, size-i-1) * (string[i] - '0');
    }
    return n;
}

char bin_to_char(int bin){
    if(bin == 1)
        return '1';
    else if (bin == 0)
        return '0';
    else
        return '\0';
}

void insert_char (char origin, char ** destine){
    //int i;
    *destine = shift_string_right(*destine, 1);
    *destine[0] = origin;
}

int string_to_file(FILE *file, char * string){
    //file must be opened
    if(!file)
        return -1;
    int i;
    int size = size_of(string);
    for(i = 0; i < size+1; i++){
        fputc(string[i], file);
    }
    return 1;
}

int string_from_file(FILE *file, char ** string){
    //file must be opened
    if(!file)
        return -1;
    int i = 0;
    *string = new_string();
    char buff;
    do{
        buff = fgetc(file);
        append(string, buff);
        i++;
    }while(buff!='\0');
    return 1;
}

void dispose_string(String *string) {
    free(*string);
}

int find_char_at_last(String string, char element){
    int size = size_of(string);
    if(string[size-1] == element)
        return 1;
    else
        return 0;
}

int find_char_at_pos(String string, char element, int index){
    int size = size_of(string);
    if(index < 0 || index >= size)
        return 0;
    if(string[index] == element)
        return 1;
    else
        return 0;
}

int equals(String str1, String str2){
    int i;
    int size1 = size_of(str1);
    int size2 = size_of(str2);
    if(size1 != size2)
        return 0;
    for(i = 0; i < size1; i++){
        if(str1[i] != str2[i])
            return 0;
    }
    return 1;
}

int remove_char(String *string, int index){
    if(!string_check_index(*string, index))
        return 0;
    int i;
    int size = size_of(*string);
    // util "size" and not "size - 1", because of '\0'
    for (i = index; i < size; i++) {
        (*string)[i] = (*string)[i+1];
    }
    // in the calling of this function, there was just one element
    // in the next calling, there will be nothing but '\0' (size == 0)
    if(size > 1)
        (*string) = (String) realloc(*string, size - 1);
    return 1;
}

int remove_last_char(String *string){
    return remove_char(string, size_of(*string) - 1);
}

