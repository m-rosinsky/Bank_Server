/*!
 * @file test_queue.c
 *
 * @brief This file contains a self-contained test battery for the
 *          generic queue implemented in source/common/queue.h
 */

#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>

#include <stdio.h>

#include "../source/common/queue.h"

/*!
 * @brief This function tests the queue_create function.
 */
void
test_queue_create (void)
{
    queue_t * p_q = queue_create();
    CU_ASSERT_PTR_NOT_NULL(p_q);

    if (NULL != p_q)
    {
        queue_destroy(p_q);
        p_q = NULL;
    }
    return;
}

/*!
 * @brief This function tests the queue_enq function.
 */
void
test_queue_enq (void)
{
    queue_t * p_q = queue_create();
    CU_ASSERT_PTR_NOT_NULL(p_q);
    if (NULL == p_q)
    {
        goto EXIT;
    }

    // Test enqueue with NULL as the queue context.
    int nums[5] = {1,2,3,4,5};
    int result = queue_enq(NULL, nums + 0);
    CU_ASSERT_EQUAL(-1, result);

    // Test enqueue with NULL as the data.
    result = queue_enq(p_q, NULL);
    CU_ASSERT_EQUAL(-1, result);

    // Test enqueue with success.
    for (size_t i = 0; i < 5; ++i)
    {
        result = queue_enq(p_q, nums + i);
        CU_ASSERT_EQUAL(0, result);
        CU_ASSERT_EQUAL(i+1, p_q->size);
    }

    EXIT:
        // Destroy queue.
        if (NULL != p_q)
        {
            queue_destroy(p_q);
            p_q = NULL;
        }
        return;
}

/*!
 * @brief This function tests the queue_deq function.
 */
void
test_queue_deq (void)
{
    queue_t * p_q = queue_create();
    CU_ASSERT_PTR_NOT_NULL(p_q);
    if (NULL == p_q)
    {
        goto EXIT;
    }

    // Test dequeue with NULL parameter.
    int * p_i = NULL;
    p_i = queue_deq(NULL);
    CU_ASSERT_PTR_NULL(p_i);

    // Enqueue with success.
    int result = -1;
    int nums[5] = {1,2,3,4,5};
    for (size_t i = 0; i < 5; ++i)
    {
        result = queue_enq(p_q, nums + i);
        CU_ASSERT_EQUAL(0, result);
        CU_ASSERT_EQUAL(i+1, p_q->size);
    }

    // Test dequeue.
    for (size_t i = 0; i < 5; ++i)
    {
        p_i = queue_deq(p_q);
        CU_ASSERT_PTR_NOT_NULL(p_i);
        if (NULL == p_i)
        {
            goto EXIT;
        }

        // Test value of p_i.
        CU_ASSERT_EQUAL(*p_i, i + 1);
    }

    EXIT:
        // Destroy queue.
        if (NULL != p_q)
        {
            queue_destroy(p_q);
            p_q = NULL;
        }
        return;
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
        {"queue_create test", test_queue_create},
        {"queue_enq test", test_queue_enq},
        {"queue_deq test", test_queue_deq},
        CU_TEST_INFO_NULL,
    };

    // Create test suites.
    CU_SuiteInfo suites[] =
    {
        {"queue test suite", NULL, NULL, NULL, NULL, tests},
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
