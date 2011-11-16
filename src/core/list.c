#include <list.h>

Vector new_vector(){
    Vector vector = (Vector) malloc(sizeof(Vector));
    vector->elements  = NULL;
    vector->elements = 0;
    vector->index = 0;
    return vector;
}

Vector new_vector_size(int size){
    Vector vector = (Vector) malloc(sizeof(Vector));
    vector->elements  = (void**) malloc(sizeof(void*)*size);
    int i;
    for(i=0;i<size;i++)
        vector->elements[i] = (void**) malloc(sizeof(void*));
    vector->size = size;
    vector->index = 0;
    return vector;
}

void dispose_vector(Vector vector){
    int i;
    if(vector){
        if(vector->elements){
            for(i = 0; i < vector->size; i++) {
                if(vector->elements[i])
                    free(vector->elements[i]);
            }  
        }
        free(vector->elements);
        free(vector);
    }
}

void vector_push(Vector vector, void* element){
    vector->elements = (void**) realloc(vector->elements, sizeof(void*)*(vector->size + 1));
    vector->elements[vector->size++] = element;
}

void* vector_next(Vector vector){
    if(vector->index + 1 <= vector->size)
        return vector->elements[vector->index++];
    else
        return NULL;
}

void* get_current(Vector vector){
    if(vector) {
        if(vector->index >= 0 && vector->index < vector->size)
            return vector->elements[vector->index];
    } 
    else
        return NULL;
}

void* vector_end(Vector vector){
    void* ret = NULL;
    void* tmp = NULL;
    while(tmp = vector_next(vector)){
        ret = tmp;
    }
    return ret;
}

void vector_reset(Vector vector){
    vector->index = 0;
}


void* vector_shift_left(Vector vector, int index){
    int i;
    void* element = NULL;
    if(index >= 0 && index < vector->size){
        element = vector->elements[index];
        for(i = index; i < vector->size; i++){
            if(vector->elements[i+1])
                vector->elements[i] = vector->elements[i+1];
        }
        return element;
    }else
        return NULL;
}

void* vector_remove(Vector vector, int index){
    void * element;
    if((element = vector_shift_left(vector, index))){
        vector->elements = (void**) realloc(vector->elements, sizeof(void*)*(vector->size--));
        return element;
    }else
        return NULL;
}

void* vector_remove_current(Vector vector){
    return vector_remove(vector, vector->index);
}

void* vector_remove_last(Vector vector){
    void* ret = NULL;
    vector_set_end(vector);
    ret = vector_remove_current(vector);
    vector_reset(vector);
    return ret;
}

void vector_reset(Vector vector){
    vector->index = 0;
}

void vector_set_end(Vector vector){
    while(vector_next(vector));
    vector->index--;
}

void* vector_end(Vector vector){
    void* ret = NULL;
    void* tmp = NULL;
    while(tmp = vector_next(vector))
        ret = tmp;
    vector_reset(vector);
    return ret;
}

