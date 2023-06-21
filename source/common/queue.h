/*!
 * @file queue.h
 *
 * @brief This file contains a generic queue implementation using
 *          a singly linked list.
 *
 *          The queue will contain references to any type of data,
 *              but will not be responsible for allocation or
 *              deallocation of referenced data.
 *
 *          Function supported are as follows:
 *
 *              - queue_create
 *              - queue_destroy
 *              - queue_enq
 *              - queue_deq
 */

#ifndef COMMON_QUEUE_H
#define COMMON_QUEUE_H

#include <stdlib.h>

/*!
 * @brief This datatype defines a node for the linked list.
 *
 * @param p_data Pointer to the referenced data.
 * @param p_next Pointer to the next node in the queue.
 */
typedef struct _queue_node queue_node_t;
struct _queue_node
{
    void * p_data;
    queue_node_t * p_next;
};

/*!
 * @brief This datatype defines a queue context.
 *
 * @param p_head The first node in the queue.
 * @param p_tail The last node in the queue.
 * @param size The number of nodes in the queue.
 */
typedef struct _queue
{
    queue_node_t * p_head;
    queue_node_t * p_tail;
    size_t size;
} queue_t;

/*!
 * @brief This function instantiates a new empty queue.
 *
 * @return Pointer to new queue context. NULL on error.
 */
queue_t *
queue_create (void);

/*!
 * @brief This function destroys a queue context.
 *
 *          This will not deallocate any data referenced by the queue.
 *              That responsiblility is left to the client since it
 *              is impossible to know if the data referenced is
 *              stack or heap allocated.
 *
 * @param[in/out] p_queue The queue context.
 *
 * @return No return value expected.
 */
void
queue_destroy (queue_t * p_queue);

/*!
 * @brief This function enqueues data into the queue.
 *
 * @param[in/out] p_queue The queue context.
 * @param[in/out] p_data The data to enqueue.
 *
 * @return 0 on success, -1 on error.
 */
int
queue_enq (queue_t * p_queue, void * p_data);

/*!
 * @brief This function dequeues the first node in the queue.
 *
 * @param[in/out] p_queue The queue context.
 *
 * @return Pointer to the data held in the first node of the queue.
 *          NULL on error or empty queue.
 */
void *
queue_deq (queue_t * p_queue);

#endif // COMMON_QUEUE_H

/***   end of file   ***/