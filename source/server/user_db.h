/*!
 * @file user_db.h
 *
 * @brief This file contains an implementation for the server's
 *          user database context.
 * 
 *          A server context will generate a user database context
 *              as a child, and use it to maintain its database
 *              throughout operation.
 */

#ifndef SERVER_USER_DB_H
#define SERVER_USER_DB_H

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"

/*!
 * @brief This datatype defines a user database entry.
 *
 * @param p_uname The username for the entry.
 * @param p_pword The password for the entry.
 * @param balance The account balance.
 * @param sid The session ID associated with the entry.
 * @param sid_time The time the sid was created.
 */
typedef struct _user_db_entry
{
    char p_uname[USER_DB_UNAME_MAX + 1];
    char p_pword[USER_DB_PWORD_MAX + 1];
    long balance;
    uint32_t sid;
    time_t sid_time;
} user_db_entry_t;

/*!
 * @brief This datatype defines a user database context.
 *
 *          The context consists of a static number of entries.
 * 
 * @param p_entries The static array holding the database entries.
 * @param num_users The number of users currently held in the database.
 */
typedef struct _user_db
{
    user_db_entry_t p_entries[USER_DB_MAX_USERS];
    int num_users;
} user_db_t;

/*!
 * @brief This function creates a new user_db context.
 *
 * @return Pointer to new user_db context. NULL on error.
 */
user_db_t *
user_db_create (void);

/*!
 * @brief This function destroys a user_db context.
 *
 * @param[in/out] p_db The database context to destroy.
 * 
 * @return No return value expected.
 */
void
user_db_destroy (user_db_t * p_db);


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
 * @return USER_DB_LOGIN_* response code.
 */
uint8_t
user_db_auth (user_db_t * p_db,
              const char * p_uname,
              const char * p_pword,
              uint32_t * p_sid);

/*!
 * @brief This function adds a new user entry to a database.
 *
 * @param[in/out] p_db The database context.
 * @param[in] p_uname The username to add.
 * @param[in] p_pword The password for the entry.
 *
 * @return USER_DB_ADD_* response code.
 */
uint8_t
user_db_add_user (user_db_t * p_db,
                  const char * p_uname,
                  const char * p_pword);

/*!
 * @brief This function removes a user with a given SID.
 *
 *          The SID must be valid.
 * 
 * @param[in/out] p_db The database context.
 * @param[in] sid The SID of the user to delete.
 * 
 * @return USER_DB_RM_* response code.
 */
uint8_t
user_db_rm_user (user_db_t * p_db, const uint32_t sid);

#endif // SERVER_USER_DB_H

/***   end of file   ***/
