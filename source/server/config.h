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

// The maxmimum and minimum length of a username and password.
#define USER_DB_UNAME_MIN 5
#define USER_DB_UNAME_MAX 30

#define USER_DB_PWORD_MIN 5
#define USER_DB_PWORD_MAX 30

// Bounds of SID values.
#define USER_DB_SID_MIN 0x00000001
#define USER_DB_SID_MAX 0xFFFFFFFE

// User login response codes.
#define USER_DB_LOGIN_SUCCESS   0xA0
#define USER_DB_LOGIN_USER_NF   0xF1
#define USER_DB_LOGIN_BAD_PASS  0xF2
#define USER_DB_LOGIN_GENFAIL   0xFF

// User add response codes (register).
#define USER_DB_ADD_SUCCESS     0xA0
#define USER_DB_ADD_USER_EX     0xF1
#define USER_DB_ADD_BAD_PASS    0xF2
#define USER_DB_ADD_DB_FULL     0xF3
#define USER_DB_ADD_GENFAIL     0xFF

#endif // SERVER_CONFIG_H

/***   end of file   ***/
