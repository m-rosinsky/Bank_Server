/*!
 * @file queue.c
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

#include "queue.h"

/*!
 * @brief This function instantiates a new empty queue.
 *
 * @return Pointer to new queue context. NULL on error.
 */
queue_t *
queue_create (void)
{
    queue_t * p_queue = calloc(1, sizeof(queue_t));
    if (NULL == p_queue)
    {
        goto EXIT;
    }
    p_queue->p_head = NULL;
    p_queue->p_tail = NULL;
    p_queue->size = 0;
    
    EXIT:
        return p_queue;
}

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
queue_destroy (queue_t * p_queue)
{
    if ((NULL == p_queue) ||
        (NULL == p_queue->p_head))
    {
        goto EXIT;
    }
    
    // Free each node in the queue.
    queue_node_t * p_curr = p_queue->p_head;
    queue_node_t * p_next = NULL;
    
    while (NULL != p_curr)
    {
        p_next = p_curr->p_next;
        free(p_curr);
        p_curr = p_next;
    }
    
    EXIT:
        if (NULL != p_queue)
        {
            free(p_queue);
            p_queue = NULL;
        }
        return;
}

/*!
 * @brief This function enqueues data into the queue.
 *
 * @param[in/out] p_queue The queue context.
 * @param[in/out] p_data The data to enqueue.
 *
 * @return 0 on success, -1 on error.
 */
int
queue_enq (queue_t * p_queue, void * p_data)
{
    int status = -1;
    if ((NULL == p_queue) ||
        (NULL == p_data))
    {
        goto EXIT;
    }
    
    // Create a new node.
    queue_node_t * p_new = calloc(1, sizeof(queue_node_t));
    if (NULL == p_new)
    {
        goto EXIT;
    }
    p_new->p_data = p_data;
    p_new->p_next = NULL;
    
    // Enqueue new node.
    if (NULL == p_queue->p_head)
    {
        p_queue->p_head = p_new;
    }
    else
    {
        p_queue->p_tail->p_next = p_new;
    }
    p_queue->p_tail = p_new;
    p_queue->size++;
    
    status = 0;
    
    EXIT:
        return status;
}

/*!
 * @brief This function dequeues the first node in the queue.
 *
 * @param[in/out] p_queue The queue context.
 *
 * @return Pointer to the data held in the first node of the queue.
 *          NULL on error or empty queue.
 */
void *
queue_deq (queue_t * p_queue)
{
    void * p_result = NULL;
    if ((NULL == p_queue) ||
        (NULL == p_queue->p_head))
    {
        goto EXIT;
    }
    
    queue_node_t * p_next = p_queue->p_head->p_next;
    p_result = p_queue->p_head->p_data;
    free(p_queue->p_head);
    p_queue->p_head = p_next;
    
    if (NULL == p_queue->p_head)
    {
        p_queue->p_tail = NULL;
    }
    p_queue->size--;
    
    EXIT:
        return p_result;
}

/***   end of file   ***/