#ifndef _OBJECT_UTIL_H
#define _OBJECT_UTIL_H
#include "object_type.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define KEY(el)        ((_key *)el.obj)
#define VECTOR(el)     ((vector *)(el.obj))
#define TABLE(el)      ((table *)(el.obj))
#define STACK(el)      ((stack *)(el.obj))
#define STR(el)        ((_string *)el.obj)



typedef enum
{

	T_NUM,
	T_CHAR,
	T_STR,
	T_BOOL,

	T_KEY,
	T_VECTOR,
	T_INCLUDE,
	T_GEN,
	T_STACK,
	T_TABLE,
	T_NULL
} obj_t;

struct _key
{
	int   hash;
	char *val;
};

struct _string
{
	int   len;
	char *String;
};

union value
{
	double Num;
	char   Char;
	bool   Bool;
};

struct vector
{
	int    count;
	int    len;
	obj_t  type;
	value *of;
};

struct buffer
{
	char *bytes;
	int   count;
	int   len;
};

struct element
{
	obj_t type;

	union
	{
		value val;
		void *obj;
	};
};



struct stack
{
	uint16_t count;
	uint16_t len;
	element *as;
};

struct record
{
	_key   *key;
	element val;
	record *next;
};

struct table
{
	uint16_t count;
	uint16_t len;
	record  *records;
};



#endif
