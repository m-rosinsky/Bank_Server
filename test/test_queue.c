/*!
 * @file test_queue.c
 *
 * @brief This file contains a self-contained test battery for the
 *          generic queue implemented in source/common/queue.h
 */

#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>

#include <stdio.h>

/*!
 * @brief This function tests the queue_create function.
 *
 * @return No return value expected.
 */
void
test_queue_create (void)
{
    CU_ASSERT_EQUAL(0, 1);
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
