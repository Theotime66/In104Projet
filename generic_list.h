/*****************************************************************************
 *                                                                            *
 *  -------------------------------- list.h --------------------------------  *
 *                                                                            *
 *****************************************************************************/

#ifndef GENERIC_LIST_H
#define GENERIC_LIST_H

#include <stdlib.h>
#include <stdbool.h>

/*****************************************************************************
 *                                                                            *
 *  Define a structure for linked list elements.                              *
 *                                                                            *
 *****************************************************************************/

typedef struct generic_list_elmt_ {

  void *data;
  struct generic_list_elmt_  *next;

} generic_list_elmt_t;

/*****************************************************************************
 *                                                                            *
 *  Define a structure for linked lists.                                      *
 *                                                                            *
 *****************************************************************************/

typedef struct generic_list_ {

  int                size;

  int                (*compare)(const void *key1, const void *key2);
  void*               (*build)(const void *data);
  void               (*destroy)(void *data);

  generic_list_elmt_t          *head;
  generic_list_elmt_t          *tail;

} generic_list_t;

/*****************************************************************************
 *                                                                            *
 *  --------------------------- Public Interface ---------------------------  *
 *                                                                            *
 *****************************************************************************/

void generic_list_init(generic_list_t *list,
		       int (*compare) (const void *key1, const void *key2),
		       void* (*build)(const void *data),
		       void (*destroy)(void *data));

void generic_list_destroy(generic_list_t *list);

int generic_list_ins_next(generic_list_t *list,
			  generic_list_elmt_t *element,
			  const void *data);

int generic_list_rem_next(generic_list_t *list,
			  generic_list_elmt_t *element,
			  void **data);

int generic_list_size (generic_list_t *list);

generic_list_elmt_t* generic_list_head (generic_list_t *list);

generic_list_elmt_t* generic_list_tail (generic_list_t *list);

bool generic_list_is_head (generic_list_t* list,
			   generic_list_elmt_t *element);

bool generic_list_is_tail (generic_list_elmt_t *element);

void* generic_list_data (generic_list_elmt_t *element);

generic_list_elmt_t* generic_list_next (generic_list_elmt_t *element);

#endif
