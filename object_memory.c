#include "common.h"
#include "object_memory.h"

static void free_entry_list(record entry);
static void free_stack(stack **stack);

vector *_vector(size_t size, obj_t type)
{
	vector *v = NULL;
	v         = ALLOC(sizeof(vector));
	v->count  = 0;
	v->len    = size;
	v->type   = type;
	v->of     = NULL;
	if (size == 0)
		size++;
	v->of = ALLOC(size * sizeof(value));
	return v;
}


stack *_stack(size_t size)
{
	stack *s = NULL;
	s        = ALLOC(sizeof(stack));

	s->as = NULL;
	s->as = ALLOC(sizeof(element) * size);

	s->count = 0;
	s->len   = (int)size;
	return s;
}
stack *realloc_stack(stack **st, size_t size)
{

	if (size == 0)
	{
		free_stack(st);
		st = NULL;
		return NULL;
	}
	if (!st)
		return _stack(size);

	if (size == (*st)->len)
		return *st;

	element *as = NULL;
	as          = ALLOC(sizeof(element) * size);

	for (size_t i = 0; i < (*st)->count; i++)
		*(as + i) = *((*st)->as + i);

	FREE((*st)->as);
	(*st)->as = NULL;
	(*st)->as = as;
	(*st)->len *= INC;
	return *st;
}

void free_vector(vector **v)
{
	if (!*v)
		return;

	FREE((*v)->of);
	(*v)->of = NULL;
	FREE(*v);
	v = NULL;
}
static void free_entry(record *entry)
{

	free_obj(&entry->val);
	free_key(&entry->key);
}
static void free_entry_list(record entry)
{
	record *tmp  = NULL;
	record *next = NULL;
	tmp          = entry.next;
	free_entry(&entry);

	while (tmp)
	{
		next = tmp->next;
		free_entry(tmp);
		tmp = next;
	}
}
void free_table(table **t)
{
	if (!*t)
		return;

	if ((*t)->count == 0)
	{
		FREE((*t)->records);
		(*t)->records = NULL;
		FREE(*t);
		t = NULL;
		return;
	}
	if (!(*t)->records)
	{
		FREE(t);
		t = NULL;
		return;
	}

	for (size_t i = 0; i < (*t)->len; i++)
		if ((*t)->records[i].key->val)
			free_entry_list((*t)->records[i]);

	FREE((*t)->records);
	(*t)->records = NULL;
	FREE(*t);
	t = NULL;
}

static void free_stack(stack **stack)
{
	if (!stack)
		return;
	if (!*stack)
	{
		stack = NULL;
		return;
	}

	if ((*stack)->count == 0)
	{
		FREE((*stack)->as);
		FREE((*stack));
		stack = NULL;
		return;
	}

	for (size_t i = 0; i < (*stack)->len; i++)
		FREE_OBJ(((*stack)->as + i));

	FREE((*stack)->as);
	(*stack)->as = NULL;
	FREE(*stack);
	stack = NULL;
}



void free_str(_string **s)
{
	if (!*s)
		return;

	if ((*s)->String)
		FREE((*s)->String);

	(*s)->String = NULL;
	FREE(*s);
	s = NULL;
}
void free_key(_key **s)
{
	if (!*s)
		return;

	if ((*s)->val)
		FREE((*s)->val);

	(*s)->val = NULL;
	FREE(*s);
	s = NULL;
}

void free_obj(element *el)
{
	switch (el->type)
	{
	case T_STR:
	{

		_string *s = NULL;
		s          = STR((*el));
		free_str(&s);
		break;
	}
	case T_KEY:
	{

		_key *k = NULL;
		k       = KEY((*el));
		free_key(&k);
		break;
	}
	case T_VECTOR:
		free_vector((vector **)&el->obj);
		break;
	case T_STACK:
		free_stack((stack **)&el->obj);
		break;
	case T_TABLE:
		free_table((table **)&el->obj);
		break;
	default:
		return;
	}
}
