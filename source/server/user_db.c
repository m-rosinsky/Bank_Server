/*!
 * @file user_db.c
 *
 * @brief This file contains an implementation for the server's
 *          user database context.
 * 
 *          A server context will generate a user database context
 *              as a child, and use it to maintain its database
 *              throughout operation.
 */

#include <string.h>

#include "user_db.h"

/*!
 * @brief This function creates a new user_db context.
 *
 * @return Pointer to new user_db context. NULL on error.
 */
user_db_t *
user_db_create (void)
{
    user_db_t * p_db = calloc(1, sizeof(user_db_t));
    if (NULL == p_db)
    {
        goto EXIT;
    }
    p_db->num_users = 0;

    // Initialize all entries.
    for (size_t idx = 0; idx < USER_DB_MAX_USERS; ++idx)
    {
        memset(p_db->p_entries + idx, '\0', sizeof(user_db_entry_t));
    }

    EXIT:
        return p_db;
}

/*!
 * @brief This function destroys a user_db context.
 *
 * @param[in/out] p_db The database context to destroy.
 * 
 * @return No return value expected.
 */
void
user_db_destroy (user_db_t * p_db)
{
    if (NULL != p_db)
    {
        free(p_db);
        p_db = NULL;
    }
    return;
}

/***   end of file   ***/
