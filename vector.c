
#include "error.h"
#include "table.h"
#include "vector.h"

void push_value(vector **v, element *obj)
{
	if ((*v)->len < (*v)->count + 1)
		*v = _realloc_vector(v, (*v)->len * INC);

	if ((*v)->type == T_GEN)
		(*v)->type = obj->type;

	else if ((*v)->type != obj->type)
		exit_error("Pushing invalid type to vector");

	*((*v)->of + (*v)->count++) = obj->val;
}

static void insert_value(vector ***v, element *obj, int index)
{

	if (((**v)->count + 1 > (**v)->len))
		**v = _realloc_vector(*v, (**v)->len * INC);
	if (index > (**v)->len)
		**v = _realloc_vector(*v, index * INC);


	if ((**v)->type == T_GEN)
		(**v)->type = obj->type;

	else if ((**v)->type != obj->type)
		exit_error(
		    "Inserting vector element at index %d with invalid type", index
		);

	(**v)->count++;

	for (int i = (**v)->count - 1; i > index; i--)
		*((**v)->of + i) = *((**v)->of + i - 1);

	*((**v)->of + index) = obj->val;

}


void _insert(vector **vect, element *obj, int index)
{
	insert_value(&vect, obj, index);
}

static inline void delete_value_index(vector ***v, Long index)
{
	if (index > (**v)->len)
		exit_error(
		    "Vector index out of range, current length: %d, provided "
		    "index: %d",
		    (**v)->len, index
		);

	for (int i = index; i < (**v)->len - 1; i++)
		*((**v)->of + i) = *((**v)->of + i + 1);

	--(**v)->count;
}


void delete_index(vector **v, Long index)
{
	delete_value_index(&v, index);
}

static inline void replace_value_index(value **of, int index, value value)
{
	*((*of) + index) = value;
}

static void set_vector_index(int index, element *obj, vector ***v)
{
	if (index > (**v)->len)
		exit_error(
		    "Vector index out of range, current length: %d, provided "
		    "index: %d",
		    (**v)->len, index
		);

	if ((**v)->type == T_GEN)
		(**v)->type = obj->type;

	else if ((**v)->type != obj->type)
		exit_error(
		    "Replacing vector element at index %d with invalid type", index
		);

	replace_value_index(&(**v)->of, index, obj->val);
}


void replace(element *i, element *obj, vector **vect)
{
	int         index = i->val.Num;
	set_vector_index(index, obj, &vect);

}
static inline element get_vector_index(int index, vector *v)
{

	if (index > v->len)
	{
		error("Array index: %d, out of bounds", index);
		return Null();
	}

	return OBJ(*(v->of + index), v->type);
}

element at(element *i, vector **obj)
{
	int    index = i->val.Num;
	return get_vector_index(index, obj);

}

static inline element pop_value(vector ***v)
{
	return ((**v)->count == 0) ? Null()
	                          : OBJ(*((**v)->of + --(**v)->count), (**v)->type);
}

element pop_obj(vector **vect)
{
	return pop_value(&vect);
}

vector *_realloc_vector(vector **v, size_t size)
{
	if (size == 0)
	{
		free_vector(v);
		return NULL;
	}
	if (!*v)
		return _vector(size, T_GEN);

	value *of = NULL;
	of        = ALLOC(sizeof(value) * size);

	for (int i = 0; i < (*v)->count; i++)
		*(of + i) = *((*v)->of + i);

	FREE((*v)->of);
	(*v)->of = NULL;
	(*v)->of = of;
	(*v)->len *= INC;

	return *v;
}

