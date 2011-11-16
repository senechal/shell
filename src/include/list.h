#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct vector{
    void ** elements;
    int size;
    int index;
}_Vector;

typedef _Vector* Vector;

Vector new_vector();

Vector new_vector_size(int size);

void dispose_vector(Vector vector);

void vector_push(Vector vector, void* element);

void* vector_next(Vector vector);

void* get_current(Vector vector);

void* vector_end(Vector vector);

void vector_reset(Vector vector);


void* vector_remove(Vector vector, int index);

void* vector_remove_last(Vector vector);

void* vector_shift_left(Vector vector, int index);

void* vector_remove_current(Vector vector);

void* vector_remove_last(Vector vector);

void vector_reset(Vector vector);

void* vector_end(Vector vector);

void vector_set_end(Vector vector);



#endif
