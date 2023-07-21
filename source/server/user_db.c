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
 * @brief Static helper function definitions.
 */

static int
user_db_find (const user_db_t * p_db, const char * p_uname);

static uint32_t
gen_sid (user_db_t * p_db, const int user_idx);

/******************************************************************************/

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

/*!
 * @brief This function authenticates an existing user, given
 *          a username and password.
 *
 *          If the authentication was successful, a new SID is
 *              generated for the user entry.
 *
 * @param[in/out] p_db The database context.
 * @param[in] p_uname The username to authenticate.
 * @param[in] p_pword The password to authenticate.
 * @param[out] p_sid The new sid generated, 0 on failure.
 *
 * @return 0 on success,
 *          1 on user not found,
 *          2 on password invalid,
 *          3 on generic failure.
 */
int
user_db_auth (user_db_t * p_db,
              const char * p_uname,
              const char * p_pword,
              uint32_t * p_sid)
{
    int status = 3; // Generic failure.
    if ((NULL == p_db) ||
        (NULL == p_uname) ||
        (NULL == p_pword) ||
        (NULL == p_sid))
    {
        goto EXIT;
    }

    // Perform a search through the database for an entry that
    // matches the username.
    int user_idx = user_db_find(p_db, p_uname);
    if (-1 == user_idx)
    {
        // User was not found, return error.
        status = 1;
        goto EXIT;
    }

    // Perform password validation.
    size_t entry_pword_len = strlen(p_db->p_entries[user_idx].p_pword);
    size_t pword_len = strnlen(p_pword, USER_DB_PWORD_MAX);
    if ((pword_len != entry_pword_len) ||
        (0 != strcmp(p_pword, p_db->p_entries[user_idx].p_pword)))
    {
        // Password did not match database entry.
        status = 2;
        goto EXIT;
    }

    // User successfully authenticated, generate new SID.
    uint32_t new_sid = gen_sid(p_db, user_idx);
    *p_sid = new_sid;

    // Set success status.
    status = 0;

    EXIT:
        return status;
}

/******************************************************************************/
/*!
 * @brief Static helper function implementations.
 */

/*!
 * @brief This function searches a database context to find an entry
 *          with a provided username.
 * 
 * @param[in] p_db The database context.
 * @param[in] p_uname The username to search.
 * 
 * @return Index in database for the user entry. -1 if not found.
 */
static int
user_db_find (const user_db_t * p_db, const char * p_uname)
{
    int user_idx = -1;
    if ((NULL == p_db) ||
        (NULL == p_uname))
    {
        goto EXIT;
    }

    // Get length of provided username.
    size_t uname_len = strnlen(p_uname, USER_DB_UNAME_MAX);

    // Perform search.
    for (int idx = 0; idx < p_db->num_users; ++idx)
    {
        size_t entry_uname_len = strlen(p_db->p_entries[idx].p_uname);
        if ((uname_len == entry_uname_len) &&
            (0 == strcmp(p_uname, p_db->p_entries[idx].p_uname)))
        {
            // Entry was found with matching username.
            user_idx = idx;
            goto EXIT;
        }
    }

    EXIT:
        return user_idx;
}

/*!
 * @brief This function generates a new SID for a given entry in
 *          a database context.
 * 
 * @param[in/out] p_db The database context.
 * @param[in] user_idx The index within the database.
 * 
 * @return The new SID generated.
 */
static uint32_t
gen_sid (user_db_t * p_db, const int user_idx)
{
    uint32_t sid = 0;
    if ((NULL == p_db) ||
        (user_idx >= p_db->num_users))
    {
        goto EXIT;
    }

    // Set the random seed.
    srand(time(NULL));

    // Generate unique random non-zero SID.
    for (;;)
    {
        sid = (((uint32_t) rand() << 16) | rand()) + 1;
        if (0 == sid) { continue; }

        // Ensure uniqueness of SID.
        int is_unique = 0;
        for (int idx = 0; idx < p_db->num_users; ++idx)
        {
            if (sid == p_db->p_entries[idx].sid)
            {
                is_unique = 1;
                break;
            }
        }

        if (0 == is_unique)
        {
            break;
        }
    }

    EXIT:
        return sid;
}

/***   end of file   ***/
