/*****************************************************************************
*                                                                            *
*  ------------------------------- queue.h --------------------------------  *
*                                                                            *
*****************************************************************************/

#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

#include "generic_list.h"

/*****************************************************************************
*                                                                            *
*  Implement queues as linked lists.                                         *
*                                                                            *
*****************************************************************************/

typedef generic_list_t generic_queue_t;

/*****************************************************************************
*                                                                            *
*  --------------------------- Public Interface ---------------------------  *
*                                                                            *
*****************************************************************************/

void generic_queue_init (generic_queue_t *queue,
			 int (*compare)(const void *key1, const void *key2),
			 void* (*build) (const void *data),
			 void (*destroy) (void *data));

void generic_queue_destroy (generic_queue_t *queue);

int generic_queue_enqueue(generic_queue_t *queue, const void *data);

int generic_queue_dequeue(generic_queue_t *queue, void **data);

void* generic_queue_peek(generic_queue_t *queue);

int generic_queue_size(generic_queue_t *queue);

#endif
