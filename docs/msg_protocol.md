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

| ![alt text](https://github.com/m-rosinsky/Bank_Server/blob/cc33e0b1971ba97f10f042602d5cf5a0a2b6f8a3/docs/imgs/msg_request_diagram.png "Request Message") |
|:--:|

This totals 256 byte messages being sent.

## Server to Client Message Protocol Diagram

| ![alt text](https://github.com/m-rosinsky/Bank_Server/blob/cc33e0b1971ba97f10f042602d5cf5a0a2b6f8a3/docs/imgs/msg_response_diagram.png "Response Message") |
|:--:|

This totals 128 byte messages being sent.

## User Register

| ![alt text](https://github.com/m-rosinsky/Bank_Server/blob/eacb7cf9933d34f6e2c7aafcdc548cb71dcf1912/docs/imgs/user_register_diagram.png "User Register") |
|:--:|

## User Delete

| ![alt text](https://github.com/m-rosinsky/Bank_Server/blob/eacb7cf9933d34f6e2c7aafcdc548cb71dcf1912/docs/imgs/user_delete_diagram.png "User Delete") |
|:--:|

## User Login

| ![alt text](https://github.com/m-rosinsky/Bank_Server/blob/eacb7cf9933d34f6e2c7aafcdc548cb71dcf1912/docs/imgs/user_login_diagram.png "User Login") |
|:--:|

## Account Balance

| ![alt text](https://github.com/m-rosinsky/Bank_Server/blob/eacb7cf9933d34f6e2c7aafcdc548cb71dcf1912/docs/imgs/account_balance_diagram.png "Account Balance") |
|:--:|

## Account Deposit

| ![alt text](https://github.com/m-rosinsky/Bank_Server/blob/eacb7cf9933d34f6e2c7aafcdc548cb71dcf1912/docs/imgs/account_deposit_diagram.png "Account Deposit") |
|:--:|

## Account Withdraw

| ![alt text](https://github.com/m-rosinsky/Bank_Server/blob/eacb7cf9933d34f6e2c7aafcdc548cb71dcf1912/docs/imgs/account_withdraw_diagram.png "Account Withdraw") |
|:--:|

## Account Transfer

| ![alt text](https://github.com/m-rosinsky/Bank_Server/blob/eacb7cf9933d34f6e2c7aafcdc548cb71dcf1912/docs/imgs/account_transfer_diagram.png "Account Transfer") |
|:--:|
