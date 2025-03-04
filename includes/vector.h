#ifndef _VECTOR_H
#define _VECTOR_H
#include "object_memory.h"

void    push_value(vector **v, element *obj);
void    _insert(vector **v, element *obj, int index);
void    delete_index(vector **v, Long index);
element pop_obj(vector **v);

void    replace(element *index, element *obj, vector **vect);
element at(element *index, vector **obj);

vector     *_realloc_vector(vector **v, size_t size);

#endif
