#include "mem.h"
#include <string.h>
#include <sys/mman.h>

#define ARM64_PAGE 16384

static _free *mem = NULL;
static void   merge_list(void);

static void *request_system_memory(size_t size)
{
	return mmap(
	    NULL, size, PROT_READ | PROT_WRITE,
	    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0
	);
}

void destroy_global_memory(void)
{

	_free *tmp = NULL;

	while (mem)
	{
		tmp = mem->next;
		munmap(mem, mem->size);
		mem = tmp;
	}
	tmp = NULL;
	mem = NULL;
}

static void merge_list(void)
{

	_free *prev = NULL;
	for (_free *next = mem; next; next = next->next)
	{

		prev = next;
		if (next->next &&
		    (((char *)next + OFFSET + next->size) == (char *)next->next))
		{
			prev->size += next->next->size;
			prev->next = next->next->next;
		}
	}
}

static inline void insert_free(_free **prev, _free **next, _free **ptr)
{
	if (*next && *next < *ptr)
	{
		(*ptr)->next  = (*next)->next;
		(*next)->next = *ptr;
	}
	else if (*prev && *prev < *ptr)
	{
		(*ptr)->next  = (*prev)->next;
		(*prev)->next = *ptr;
	}

}

void _free_(void *new)
{

	if (!new)
		return;
	_free *ptr = NULL;
	ptr        = PTR(new);

	if (!ptr)
		return;
	if (ptr->size == 0)
		return;

	_free *next = NULL, *prev = NULL;

	for (next = mem; next->next && next < ptr; next = next->next)
		prev = next->next;

	insert_free(&prev, &next, &ptr);
	merge_list();

	ptr  = NULL;
	new  = NULL;
	prev = NULL;
	next = NULL;
}

static inline void *init_alloced_ptr(void *ptr, size_t size)
{
	_free *alloced = NULL;
	alloced        = ptr;
	alloced->size  = size - OFFSET;
	alloced->next  = NULL;

	return 1 + alloced;
}
static inline void init_free_ptr(_free **ptr, size_t alloc_size, size_t size)
{
	(*ptr)->next       = request_system_memory(alloc_size);
	(*ptr)->next->size = alloc_size - size - OFFSET;
	(*ptr)->next->next = NULL;
}

static void alloc_free_list(void)
{
	mem       = request_system_memory(ARM64_PAGE);
	mem->size = ARM64_PAGE - OFFSET;
	mem->next = NULL;
}

static inline void detach(_free **prev, _free **next)
{
	if (*prev && (*next)->size == 0)
		(*prev)->next = (*next)->next;
	else if (!*prev && (*next)->size == 0)
		mem = (*next)->next;
}

void *_malloc_(size_t size)
{

	if (!mem)
		alloc_free_list();

	_free *prev = NULL;
	_free *next = NULL;

	for (next = mem; next && next->size < size; next = next->next)
		prev = next;

	if (next && next->size >= size)
	{
		next->size -= size;
		detach(&prev, &next);
		return init_alloced_ptr(FPTR(next) + next->size, size);
	}

	if (prev)
	{
		size_t tmp = ARM64_PAGE;
		while (size > tmp)
			tmp *= INC;
		init_free_ptr(&prev, tmp, size);
		return init_alloced_ptr(
		    FPTR(prev->next) + prev->next->size, size
		);
	}

	return NULL;
}

void *_calloc_(int val, size_t size)
{
	return memset(ALLOC(size), val, size);
}

void *_realloc_(void *ptr, size_t old_size, size_t size)
{

	if (!ptr && size != 0)
		return ALLOC(size);

	if (!ptr)
		return NULL;

	if (size == 0)
	{
		FREE(ptr);
		ptr = NULL;
		return NULL;
	}

	void *alloced = NULL;
	alloced       = ALLOC(size);

	memmove(alloced, ptr, old_size);
	FREE(ptr);

	return alloced;
}
