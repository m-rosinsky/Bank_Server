# Bank_Server

A banking server and client written from scratch in C.

## Objectives

The main objective of this project is to practice project architecture
by creating a multi-component project from the ground up.

Rules should be put in place for coding standards, documentation,
workflow, merge requests, etc.

## Project Specifications

This project will consist of 2 major components:

- Server
- Client

The server will act as a banking database, holding account info for all
clients.

Client's will be able to query the server to perform the following tasks:

- Create an account
- Close an account
- Query balance
- Deposit funds
- Withdraw funds
- Transfer funds to another user

The server should be able to handle multiple client's simulataneously.

The client program should act as an interface for sending and receiving
queries to the server. The client interface should be user-friendly,
and offer help when needed.

## Coding Standards

1. All code shall abide by Barr-C coding standards.
2. All code shall be documented with doxygen inline commenting.
