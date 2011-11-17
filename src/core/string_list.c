 
#include <string_list.h>
#include <stdarg.h>
#include <stdio.h>

// teste tad lista
StringList new_string_list(){
    StringList list;
    list.string = NULL;
    list.size = 0;
    list.index = 0;
    return list;
}

//int add(StringList list, String element, int index);

// sem ultimo elemento apontando p null
int list_append(StringList *list, String element) {
    list->string = (String*) realloc(list->string,sizeof(String)*(list->size+1));
    if (!list->string)
        return 0;
    list->string[list->size] = copy_string(element);
    list->size++;
    return 1;
}
//Quebra uma String e cria um StringList, separando os comandos, do parser
StringList explode(String string, char separator){
    StringList list = new_string_list();
    int i;
    int size = size_of(string);
    String buffer = new_string();
    // loop in string
    for(i = 0; i <= size; i++){
        if(string[i] != separator && i < size)
            append(&buffer, string[i]);
        else{
            list_append(&list, buffer);
            dispose_string(&buffer);
            buffer = new_string();
        }
    }
    dispose_string(&buffer);
    return list;
}

int list_find_char_at_last(StringList list, char element){
    String tmp;
    reset_index(&list);
    while((tmp = list_next(&list))){
        if(find_char_at_last(tmp, element))
            return list.index - 1;
    }
    return -1;
}
//remove da lista strings vazias
void trim_string_list(StringList *list, char element){
    String tmp;
    while((tmp = list_next(list))){
        if(is_empty_string(tmp)){
            list_remove(list, --list->index);
        }
    }
    reset_index(list);
}

int is_valid_index(StringList list, int index){
    if(index >= 0 && index < list.size)
        return 1;
    else
        return 0;
}

int list_remove(StringList *list, int index){
    int i;
    if(is_valid_index(*list, index)){
        // for each one until the end, copy the string
        for(i = index; i < list->size - 1; i++)
            list->string[i] = copy_string(list->string[i+1]);
        // and removes the last
        dispose_string(&list->string[list->size-1]);
        list->string = (String*) realloc(list->string, sizeof(String)*(--list->size));
        return 1;
    }else
        return 0;
}

int list_remove_current(StringList *list){
    return list_remove(list, list->index);
}

String list_current(StringList list){
    return list.string[list.index];
}
//retorna o proximo item de uma stringList
String list_next(StringList * list){
    if(list->index + 1 <= list->size)
        return list->string[list->index++];
    else
        return NULL;
}

int set_index(StringList *list, int index){
    if(index >= 0 && index < list->size){
        list->index = index;
        return 1;
    }else
        return 0;
}

void reset_index(StringList *list){
    list->index = 0;
}

String get_string(StringList list, int index){
    if(index < list.size && index >= 0)
        return list.string[index];
    else
        return NULL;
}

void dispose_string_list(StringList *list){
    while(list->size > 0) { 
        dispose_string(&list->string[--list->size]);
        if(list->size > 0)
            list->string = (String*) realloc(list->string,sizeof(String)*list->size);
        else
            free(list->string);
    }    
    list->string = NULL;
}

void print_list(StringList list) {
    String tmp;
    while((tmp = list_next(&list))){
        printf("%s\n", tmp);
    }
}

// last array element must be NULL
StringList from_array(char * array[]){
    int i = 0;
    StringList list = new_string_list();
    String string;
    while(array[i] != NULL){
        string = copy_string(array[i]);
        list_append(&list, string);
        i++;
    }
    return list;
}

StringList from_array_size(char * array[], int size){
    int i;
    StringList list = new_string_list();
    String string;
    for(i = 0; i < size; i++){
        if(array[i] != NULL){
            string = copy_string(array[i]);
            list_append(&list, string);
            i++;
        }
    }
    return list;
}

StringList copy_list(StringList list){
    StringList copy = new_string_list();
    String string;
    while((string = list_next(&list))){
        list_append(&copy, string);
    }
    return copy;
}

char ** to_array(StringList list){
    char ** array = (char**) malloc(sizeof(char*)*(list.size+1));
    String string;
    int i = 0;
    while((string = list_next(&list))){
        array[i] = copy_string(string);
        i++;
    }
    array[i] = NULL;
    return array;
}

int find_string(StringList list, String str) {
    int n = 0; 

    return n; 
}

int * number_string(StringList list, String str) {
    int * a = NULL; 

    return a; 
}
