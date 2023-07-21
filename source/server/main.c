/*!
 * @file source/server/main.c
 *
 * @brief This file contains the entry point for the server program.
 */

#include <stdio.h>
#include <stdint.h>

#include "user_db.h"

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

    // Perform auth with non-existant username.
    char * p_uname = "user1";
    char * p_pword = "password";
    uint32_t sid = 0;

    if (1 != user_db_auth(p_db, p_uname, p_pword, &sid))
    {
        printf("auth no-exist\n");
        return 1;
    }

    user_db_destroy(p_db);

    printf("success\n");
    return 0;
}

/***   end of file   ***/
