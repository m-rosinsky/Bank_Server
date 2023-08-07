/*!
 * @file test_user_db.c
 *
 * @brief This file contains a self-contained test battery for the
 *          user database implemented in source/server/user_db.h
 */

#include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>

#include <stdio.h>
#include <stdint.h>

#include "../source/server/user_db.h"
#include "../source/server/config.h"

/*!
 * @brief Global variables for testing.
 */
user_db_t * gp_db;

/*!
 * @brief This function is the initializer for the test battery.
 *          It initializes the user db context used through the test.
 * 
 * @return 0.
 */
int
test_db_init (void)
{
    // Initialize the global variable.
    gp_db = user_db_create();
    CU_ASSERT_PTR_NOT_NULL_FATAL(gp_db);

    return 0;
}

/*!
 * @brief This function is the clean for the test battery.
 *          It destroys the user db context used through testing.
 * 
 * @return 0.
 */
int
test_db_clean (void)
{
    user_db_destroy(gp_db);
    gp_db = NULL;
    return 0;
}

/*!
 * @brief This function dumps contents of a user database context to
 *          stdout for testing purposes.
 */
void
display_user_db (user_db_t * p_db)
{
    if ((NULL == p_db) ||
        (NULL == p_db->p_entries))
    {
        goto EXIT;
    }

    for (int i = 0; i < p_db->num_users; ++i)
    {
        printf("\n");
        user_db_entry_t entry = p_db->p_entries[i];
        printf("[%d] p_uname : '%s'\n", i, entry.p_uname);
        printf("[%d] p_pword : '%s'\n", i, entry.p_pword);
        printf("[%d] balance : '%ld'\n", i, entry.balance);
        printf("[%d] sid     : '%u'\n", i, entry.sid);
        printf("[%d] sid_time: '%ld'\n", i, entry.sid_time);
    }

    EXIT:
        return;
}

/*!
 * @brief This function tests the user_db_add_user functionality.
 */
void
test_db_add_user (void)
{
    uint8_t status = 0;

    // Add a valid user to the database.
    char * p_uname1 = "user1";
    char * p_pword1 = "password1";
    status = user_db_add_user(gp_db, p_uname1, p_pword1);
    CU_ASSERT_EQUAL(USER_DB_ADD_SUCCESS, status);

    // Add duplicate user.
    status = user_db_add_user(gp_db, p_uname1, p_pword1);
    CU_ASSERT_EQUAL(USER_DB_ADD_USER_EX, status);

    // Add second user.
    char * p_uname2 = "user2";
    char * p_pword2 = "password2";
    status = user_db_add_user(gp_db, p_uname2, p_pword2);
    CU_ASSERT_EQUAL(USER_DB_ADD_SUCCESS, status);

    // Add user with invalid password (too short).
    char * p_uname3 = "user3";
    char * p_pword3 = "hi";
    status = user_db_add_user(gp_db, p_uname3, p_pword3);
    CU_ASSERT_EQUAL(USER_DB_ADD_BAD_PASS, status);

    // Add user with invalid password (too long).
    char * p_uname4 = "user4";
    char p_pword4[USER_DB_PWORD_MAX + 2];
    memset(p_pword4, '\0', USER_DB_PWORD_MAX + 2);
    memset(p_pword4, 'A', USER_DB_PWORD_MAX + 1);
    status = user_db_add_user(gp_db, p_uname4, p_pword4);
    CU_ASSERT_EQUAL(USER_DB_ADD_BAD_PASS, status);

    // Dump contents.
    display_user_db(gp_db);

    return;
}

/*!
 * @brief This function tests the user_db_auth functionality.
 */
void
test_db_auth (void)
{
    uint8_t status = 0;

    // Login as valid user and get sid.
    uint32_t sid = 0;
    char * p_uname1 = "user1";
    char * p_pword1 = "password1";
    status = user_db_auth(gp_db, p_uname1, p_pword1, &sid);
    CU_ASSERT_EQUAL(USER_DB_LOGIN_SUCCESS, status);
    CU_ASSERT_EQUAL(gp_db->p_entries[0].sid, sid);

    // Login as user that does not exist.
    char * p_uname2 = "this user does not exist";
    char * p_pword2 = "password";
    status = user_db_auth(gp_db, p_uname2, p_pword2, &sid);
    CU_ASSERT_EQUAL(USER_DB_LOGIN_USER_NF, status);
    CU_ASSERT_EQUAL(0, sid);

    // Login as existing user with wrong password.
    char * p_pword3 = "wrong password";
    status = user_db_auth(gp_db, p_uname1, p_pword3, &sid);
    CU_ASSERT_EQUAL(USER_DB_LOGIN_BAD_PASS, status);
    CU_ASSERT_EQUAL(0, sid);

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
        {"user db test add user", test_db_add_user},
        {"user db test auth", test_db_auth},
        CU_TEST_INFO_NULL,
    };

    // Create test suites.
    CU_SuiteInfo suites[] =
    {
        {"user db test suite", test_db_init, test_db_clean, NULL, NULL, tests},
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
