/*!
 * @file threadpool.c
 *
 * @brief This file contains a generic threadpool implementation.
 *
 *          The threadpool is designed to create a set number of threads
 *              upon instantiation, and maintains a queue of jobs
 *              that are passed off to any available threads.
 *
 *          When the threadpool is destroyed, threads will be allowed
 *              to finish out any work on the job queue, then be
 *              joined to the main thread for destruction.
 */

#include "threadpool.h"

/*!
 * @brief This is a static function that defines the behavior of
 *          an inactive thread in the threadpool.
 *
 *          The thread will wait for a job to be enqueued, pick up the job,
 *              perform the job, then release the job's memory.
 *
 *          If no job is available for the thread, the thread will
 *              enter a wait state dependent on the threadpool's
 *              condition variable.
 *
 *          The thread will exit this function when the threadpool's
 *              shutdown signal is asserted and there are no jobs
 *              remaining to pick up from the queue.
 *
 * @param[in/out] vp_tp A void pointer to the thread's parent threadpool
 *                  context. This is a void pointer to be in compliance with
 *                  the pthread_create function's specs.
 *
 * @return No return value expected.
 */
static void *
threadpool_inactive (void * vp_tp)
{
    if (NULL == vp_tp)
    {
        goto EXIT;
    }
    
    // Cast the void pointer to its appropriate type.
    threadpool_t * p_tp = (threadpool_t *) vp_tp;
    
    // This holds the current job the thread is performing.
    job_t * p_job = NULL;
    
    // Enter main inactivity loop.
    for (;;)
    {
        // Enter critical section.
        pthread_mutex_lock(&(p_tp->mutex));
        
        // Check if the job queue is empty.
        if (0 == p_tp->p_queue->size)
        {
            // If the shutdown signal is asserted, we can exit.
            if (true == p_tp->b_shutdown)
            {
                pthread_mutex_unlock(&(p_tp->mutex));
                goto EXIT;
            }
            
            // Enter a wait state on the condition variable.
            pthread_cond_wait(&(p_tp->cond), &(p_tp->mutex));
        }
        
        // Pick up a job from the job queue.
        p_job = queue_deq(p_tp->p_queue);
        
        // Exit critical section.
        pthread_mutex_unlock(&(p_tp->mutex));
        
        // If the job is NULL here, something went wrong in the dequeue
        // function. Just ignore and continue.
        if (NULL == p_job)
        {
            continue;
        }
        
        // Perform the job.
        p_job->job_func(&(p_tp->b_shutdown), p_job->p_arg);
        
        // Free the job and set to NULL.
        free(p_job);
        p_job = NULL;
    }
    
    EXIT:
        return NULL;
}

/*!
 * @brief This function instantiates a new threadpool context.
 *
 * @param[in] num_threads The number of threads in the job queue.
 *              This cannot be changed after instantiation.
 *              This number must be non-zero, or error will be returned.
 *
 * @return Pointer to new threadpool context. NULL on error.
 */
threadpool_t *
threadpool_create (const size_t num_threads)
{
    int status = -1;
    threadpool_t * p_tp = calloc(1, sizeof(threadpool_t));
    if ((NULL == p_tp) ||
        (0 == num_threads))
    {
        goto EXIT;
    }
    p_tp->p_threads = NULL;
    p_tp->num_threads = num_threads;
    p_tp->b_shutdown = false;
    p_tp->p_queue = NULL;
    
    // Initialize the mutex and condition variable.
    if ((0 != pthread_mutex_init(&(p_tp->mutex), NULL)) ||
        (0 != pthread_cond_init(&(p_tp->cond), NULL)))
    {
        goto EXIT;
    }
    
    // Create the job queue.
    p_tp->p_queue = queue_create();
    if (NULL == p_tp->p_queue)
    {
        goto EXIT;
    }
    
    // Allocate space for the inidividual threads.
    p_tp->p_threads = calloc(num_threads, sizeof(pthread_t));
    if (NULL == p_tp->p_threads)
    {
        goto EXIT;
    }
    
    // Initialize the threads into the inactive function.
    for (size_t tid = 0; tid < num_threads; ++tid)
    {
        if (0 != pthread_create(p_tp->p_threads + tid, NULL, threadpool_inactive, p_tp))
        {
            goto EXIT;
        }
    }
    
    status = 0;
    
    EXIT:
        if ((-1 == status) &&
            (NULL != p_tp))
        {
            threadpool_destroy(p_tp);
            p_tp = NULL;
        }
        return p_tp;
}

/*!
 * @brief This function destroys a threadpool context.
 *
 *          Any jobs remaining on the threadpool's job queue will
 *              be allowed to finish before the threads exit
 *              and cleanup is conducted.
 *
 * @param[in/out] p_tp The threadpool context.
 *
 * @return 0 on success, -1 on error.
 */
int
threadpool_destroy (threadpool_t * p_tp)
{
    int status = -1;
    if (NULL == p_tp)
    {
        goto EXIT;
    }
    
    // Assert the threadpool's shutdown signal.
    p_tp->b_shutdown = true;
    
    // Send a broadcast signal on the threadpool's condition variable
    // to release any waiting threads.
    if (0 != pthread_cond_broadcast(&(p_tp->cond)))
    {
        goto EXIT;
    }
    
    // Join all individual threads.
    for (size_t tid = 0; tid < p_tp->num_threads; ++tid)
    {
        if (0 != pthread_join(p_tp->p_threads[tid], NULL))
        {
            goto EXIT;
        }
    }
    
    // Free the array containing the threads.
    free(p_tp->p_threads);
    p_tp->p_threads = NULL;
    
    // Destroy the job queue.
    queue_destroy(p_tp->p_queue);
    p_tp->p_queue = NULL;
    
    // Destroy the mutex and condition variables.
    if ((0 != pthread_mutex_destroy(&(p_tp->mutex))) ||
        (0 != pthread_cond_destroy(&(p_tp->cond))))
    {
        goto EXIT;
    }
    
    status = 0;
    
    EXIT:
        if (NULL != p_tp)
        {
            free(p_tp);
            p_tp = NULL;
        }
        return status;
}

/*!
 * @brief This function enqueues a job on the threadpool.
 *
 * @param[in/out] p_tp The threadpool context.
 * @param[in] job_func The function to perform.
 * @param[in] p_arg The arguments associated with the job.
 *
 * @return 0 on success, -1 on error.
 */
int
threadpool_enq (threadpool_t * p_tp, job_f job_func, void * p_arg)
{
    int status = -1;
    if ((NULL == p_tp) ||
        (NULL == p_tp->p_queue) ||
        (NULL == job_func))
    {
        goto EXIT;
    }
    
    // Create the new job context.
    job_t * p_new = calloc(1, sizeof(job_t));
    if (NULL == p_new)
    {
        goto EXIT;
    }
    p_new->job_func = job_func;
    p_new->p_arg = p_arg;
    
    // Enter critical section.
    pthread_mutex_lock(&(p_tp->mutex));
    
    // Enqueue the job.
    if (-1 == queue_enq(p_tp->p_queue, p_new))
    {
        pthread_mutex_unlock(&(p_tp->mutex));
        goto EXIT;
    }
    
    // Exit critical section.
    pthread_mutex_unlock(&(p_tp->mutex));
    
    // Send a signal to the threadpool's condition variable to release
    // a thread to handle the job.
    if (0 != pthread_cond_signal(&(p_tp->cond)))
    {
        goto EXIT;
    }
    
    status = 0;
    
    EXIT:
        if ((-1 == status) &&
            (NULL != p_new))
        {
            free(p_new);
            p_new = NULL;
        }
        return status;
}

/***   end of file   ***/