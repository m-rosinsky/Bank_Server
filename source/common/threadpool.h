/*!
 * @file threadpool.h
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
 *
 *          Functions included are as follows:
 *
 *              - threadpool_create
 *              - threadpool_destroy
 *              - threadpool_enq
 */

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>

#include "queue.h"

/*!
 * @brief This datatype defines a threadpool context.
 *
 * @param p_threads The array containing the individual threads.
 * @param num_threads The number of threads in the threadpool.
 * @param b_shutdown The threadpool's shutdown signal.
 * @param mutex The threadpool mutex.
 * @param cond The threadpool condition variable.
 * @param p_queue The threadpool job queue.
 */
typedef struct _threadpool
{
    pthread_t *     p_threads;
    size_t          num_threads;
    _Atomic bool    b_shutdown;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    queue_t *       p_queue;
} threadpool_t;

/*!
 * @brief This datatype defines a function template for a job that the
 *          threadpool can perform.
 *
 * @param pb_shutdown Pointer to the parent threadpool's shutdown signal.
 * @param p_arg The arguments to be passed to the job function.
 *
 * @return No return value expected.
 */
typedef void (*job_f)(_Atomic bool * pb_shutdown, void * p_arg);

/*!
 * @brief This datatype defines a job function packaged with its
 *          arguments to be enqueued into a threadpool.
 *
 * @param job_func The job function pointer.
 * @param p_arg The job arguments.
 */
typedef struct _job
{
    job_f job_func;
    void * p_arg;
} job_t;

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
threadpool_create (const size_t num_threads);

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
threadpool_destroy (threadpool_t * p_tp);

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
threadpool_enq (threadpool_t * p_tp, job_f job_func, void * p_arg);

#endif // THREADPOOL_H

/***   end of file   ***/