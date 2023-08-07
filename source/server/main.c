/*!
 * @file source/server/main.c
 *
 * @brief This file contains the entry point for the server program.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "user_db.h"
#include "config.h"

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
        user_db_entry_t entry = p_db->p_entries[i];
        printf("[%d] p_uname : '%s'\n", i, entry.p_uname);
        printf("[%d] p_pword : '%s'\n", i, entry.p_pword);
        printf("[%d] balance : '%ld'\n", i, entry.balance);
        printf("[%d] sid     : '%u'\n", i, entry.sid);
        printf("[%d] sid_time: '%ld'\n\n", i, entry.sid_time);
    }

    EXIT:
        return;
}

/*!
 * @brief This is the entry point for the server program. It handles
 *          parsing of command line arguments and dispatch of the main
 *          server context.
 * 
 * @return 0 on success, 1 on error.
 */
int
main ()
{
    user_db_t * p_db = user_db_create();
    if (NULL == p_db)
    {
        printf("create\n");
        return 1;
    }

    uint8_t status = 0;

    // Add user.
    char * p_uname1 = "user1";
    char * p_pword1 = "password1";
    status = user_db_add_user(p_db, p_uname1, p_pword1);
    if (USER_DB_ADD_SUCCESS != status)
    {
        printf("add user1 (%x)\n", status);
        return 1;
    }

    // Add duplicate user.
    status = user_db_add_user(p_db, p_uname1, p_pword1);
    if (USER_DB_ADD_USER_EX != status)
    {
        printf("add dup (%x)\n", status);
        return 1;
    }

    // Add second user.
    char * p_uname2 = "user2";
    char * p_pword2 = "password2";
    status = user_db_add_user(p_db, p_uname2, p_pword2);
    if (USER_DB_ADD_SUCCESS != status)
    {
        printf("add user2 (%x)\n", status);
        return 1;
    }

    // Add user with invalid password (too short).
    char * p_uname3 = "user3";
    char * p_pword3 = "hi";
    status = user_db_add_user(p_db, p_uname3, p_pword3);
    if (USER_DB_ADD_BAD_PASS != status)
    {
        printf("add user3 (%x)\n", status);
        return 1;
    }

    // Add user with invalid password (too long).
    char * p_uname4 = "user4";
    char p_pword4[USER_DB_PWORD_MAX + 2];
    memset(p_pword4, '\0', USER_DB_PWORD_MAX + 2);
    memset(p_pword4, 'A', USER_DB_PWORD_MAX + 1);
    status = user_db_add_user(p_db, p_uname4, p_pword4);
    if (USER_DB_ADD_BAD_PASS != status)
    {
        printf("add user4 (%x)\n", status);
        return 1;
    }

    display_user_db(p_db);

    user_db_destroy(p_db);

    printf("success\n");
    return 0;
}

/***   end of file   ***/
