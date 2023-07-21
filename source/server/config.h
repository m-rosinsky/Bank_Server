/*!
 * @file server/config.h
 *
 * @brief This file contains compile-time configuration options for
 *          the server.
 */

#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

/*!
 * @brief User database configurations
 */

// The maximum number of users.
#define USER_DB_MAX_USERS 50

// The maxmimum length of a username and password.
#define USER_DB_UNAME_MAX 30
#define USER_DB_PWORD_MAX 30

// Bounds of SID values.
#define USER_DB_SID_MIN 0x00000001
#define USER_DB_SID_MAX 0xFFFFFFFE

#endif // SERVER_CONFIG_H

/***   end of file   ***/
