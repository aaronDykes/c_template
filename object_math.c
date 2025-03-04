#include "error.h"
#include "object_math.h"
#include "object_string.h"
#include <math.h>

element _add(element *a, element *b)
{

	switch (b->type)
	{
	case T_NUM:
		return Num(b->val.Num + a->val.Num);
	case T_CHAR:
		return Char(b->val.Char + a->val.Num);
	case T_STR:
		return append(b, a);
	default:
		error("Invalid addition operation");
		exit(1);
	}
}
element _to_str(element *a)
{
	switch (a->type)
	{
	case T_NUM:
		return lltoa(a->val.Num);
	case T_CHAR:
		return char_to_str(a->val.Char);
	case T_VECTOR:
		return vector_to_str(VECTOR((*a)));
	default:
		error("Invalid string conversion");
		exit(1);
	}
}

element _len(element *a)
{

	switch (a->type)
	{
	case T_STR:
		return Num(STR((*a))->len);
	case T_VECTOR:
		return Num(VECTOR((*a))->count);
	default:
		error("Unable to get length of invalid object");
		exit(1);
	}
}
