/*!
 * @file test_threadpool.c
 *
 * @brief This file contains a self-contained test battery for the
 *          generic threadpool implemented in source/common/threadpool.h
 */

#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>

#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <unistd.h>

#include "../source/common/threadpool.h"

#define NUM_THREADS 5

threadpool_t * p_tp = NULL;

/*!
 * @brief This function creates a threadpool for the init test battery.
 */
static int
test_threadpool_init (void)
{
    p_tp = threadpool_create(NUM_THREADS);
    CU_ASSERT_PTR_NOT_NULL(p_tp);
    return 0;
}

/*!
 * @brief This function destroys a threadpool for the cleanup test battery.
 */
static int
test_threadpool_clean (void)
{
    int result = threadpool_destroy(p_tp);
    CU_ASSERT_NOT_EQUAL(-1, result);
    return 0;
}

/*!
 * @brief This is a dummy function for a threadpool job.
 */
static void
inc (_Atomic bool * pb_shutdown, _Atomic int * p_int)
{
    CU_ASSERT_PTR_NOT_NULL(p_int);
    CU_ASSERT_PTR_NOT_NULL(pb_shutdown);
    (*p_int)++;
}

/*!
 * @brief This function tests a threadpool by adding 100 jobs.
 */
#define INCR_ROUNDS 100
static void
test_threadpool_inc (void)
{
    CU_ASSERT_PTR_NOT_NULL(p_tp);
    _Atomic int incr = 0;
    int result = -1;
    for (size_t i = 0; i < INCR_ROUNDS; ++i)
    {
        result = threadpool_enq(p_tp, (job_f) inc, &incr);
        CU_ASSERT_NOT_EQUAL(-1, result);
    }

    // Allow jobs to complete before printing.
    sleep(1);

    printf("incr: %d\n", incr);
    CU_ASSERT_EQUAL(INCR_ROUNDS, incr);
}

int
main ()
{
    // Initialize the CUnit test registry.
    if (CUE_SUCCESS != CU_initialize_registry())
    {
        goto EXIT;
    }

    // Set verbose mode.
    CU_basic_set_mode(CU_BRM_VERBOSE);

    // Create test battery array.
    CU_TestInfo tests[] =
    {
        {"threadpool increment test", test_threadpool_inc},
        CU_TEST_INFO_NULL,
    };

    // Create test suites.
    CU_SuiteInfo suites[] =
    {
        {"threadpool test suite", test_threadpool_init, test_threadpool_clean, NULL, NULL, tests},
        CU_SUITE_INFO_NULL,
    };

    // Register suites.
    if (CUE_SUCCESS != CU_register_suites(suites))
    {
        fprintf(stderr, "Register suites failed - %s\n", CU_get_error_msg());
        goto EXIT;
    }

    // Run basic tests.
    CU_basic_run_tests();

    EXIT:
        CU_cleanup_registry();
        return CU_get_error();
}

/***   end of file   ***/
