# Message Protocol

## Metadata
```
Title: Message Protocol
Data Created: 6/20/2023
Last Updated: 6/20/2023
Last Updated by: m-rosinsky
Purpose: Describe messaging protocol between client and server modules.
```

## Client to Server Messages

The messages that can be sent from the client to the server are as follows:

- user_register
- user_delete
- user_login
- account_balance
- account_deposit
- account_withdraw
- account_transfer

## Client to Server Message Protocol Diagram

| 8 | 32 | 32 | 32 | 64 | 88 |
| --- | --- | --- | --- | --- | --- |
| opcode | sid | username | password | amount | padding |

This totals 256 byte messages being sent.

## Server to Client Message Protocol Diagram

| 8 | 32 | 64 | 24 |
| --- | --- | --- | --- |
| response code | sid | amount | padding |

This totals 128 byte messages being sent.

## User Register

The user_register message is sent when creating a user account.

The message fields are as follows:

- opcode: 0x01
- sid: N/A
- username: username provided by client
- password: password provided by client
- amount: N/A

The server will respond:

- response code: 0xA0 (success), 0xF1 (user already exists), 0xF2 (password invalid), 0xF3 (database full), 0xFF (generic failure)
- sid: N/A
- amount: N/A

## User Delete

The user_delete message is sent when removing a user account.

The message fields are as follows:

- opcode: 0x02
- sid: Valid SID
- username: N/A
- password: N/A
- amount: N/A

The server will respond:

- response code: 0xA0 (success), 0xF0 (invalid sid), 0xFF (generic failure)
- sid: N/A
- amount: N/A

## User Login

The user_login message is sent when a user attempts to log in to their account.

The message fields are as follows:

- opcode: 0x03
- sid: N/A
- username: username provided by client
- password: password provided by client
- amount: N/A

The server will respond:

- response code: 0xA0 (success), 0xF1 (user not found), 0xF2 (password invalid), 0xFF (generic failure)
- sid: New generated sid
- amount: N/A
