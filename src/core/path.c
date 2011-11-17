#include "path.h"

String get_path(char * env[]){
    StringList envp = from_array(env);
    String string, aux;
    while((string = list_next(&envp))){
       aux = copy_substring(string, 0, 3);
       if(equals(aux, "PATH")){
           string = copy_substring(string, 5, size_of(string)-1);
           dispose_string_list(&envp);
           return string;
       }
    }
    dispose_string_list(&envp);
    return new_string();
}

StringList get_path_array(char *env[]){
    StringList path_array = explode(get_path(env), ':');
    return path_array;
}
//
StringList path_concat(char * env[], String command){
    String string;
    StringList path_array = get_path_array(env);
    StringList path_conc = new_string_list();
    while((string = list_next(&path_array))){
        append(&string, '/');
        append_string(&string, command);
        list_append(&path_conc, string);
    }
    return path_conc;
}

int is_absolute(String path){
    if(path[0] == '/')
        return ABSOLUTE;
    else
        return RELATIVE;
}

