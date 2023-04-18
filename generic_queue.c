/*****************************************************************************
*                                                                            *
*  ------------------------------- queue.c --------------------------------  *
*                                                                            *
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "generic_queue.h"

#ifdef DEBUG
#pragma message "activate debug mode generic_queue"
#endif

/*****************************************************************************
*                                                                            *
*  ------------------------------ queue_init ------------------------------  *
*                                                                            *
*****************************************************************************/

void generic_queue_init (generic_queue_t *queue,
			 int (*compare)(const void *key1, const void *key2),
			 void* (*build) (const void *data),
			 void (*destroy) (void *data))
{
  generic_list_init (queue, compare, build, destroy);
}

/*****************************************************************************
*                                                                            *
*  ------------------------------ queue_destroy ---------------------------  *
*                                                                            *
*****************************************************************************/

void generic_queue_destroy (generic_queue_t *queue)
{
  generic_list_destroy (queue);
}

/*****************************************************************************
*                                                                            *
*  ----------------------------- queue_enqueue ----------------------------  *
*                                                                            *
*****************************************************************************/

int generic_queue_enqueue(generic_queue_t *queue, const void *data) {

  /* Enqueue the data. */
  return generic_list_ins_next(queue, generic_list_tail(queue), data);
}

/*****************************************************************************
*                                                                            *
*  ----------------------------- queue_dequeue ----------------------------  *
*                                                                            *
*****************************************************************************/

int generic_queue_dequeue(generic_queue_t *queue, void **data) {

  /* Dequeue the data. */
  return generic_list_rem_next(queue, NULL, data);
}

/*****************************************************************************
*                                                                            *
*  ------------------------------ queue_peek ------------------------------  *
*                                                                            *
*****************************************************************************/

void* generic_queue_peek(generic_queue_t *queue) {

  /* Get the data on the top of the queue. */
  if (queue->size == 0) {
    return NULL;
  }
  else {
    return queue->head->data;
  }
}

/*****************************************************************************
*                                                                            *
*  ------------------------------ queue_size ------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_queue_size(generic_queue_t *queue) {
  return generic_list_size (queue);
}
