# TCP Listener

This is a simple TCP listener implemented in C that listens on port 80 and logs client access to a text file.

## Table of Contents
- [Features](#features)
- [Prerequisites](#prerequisites)
- [How to Compile](#how-to-compile)
- [How to Run](#how-to-run)
- [Usage](#usage)

## Features

- Listens on port 80 for incoming connections.
- Logs client access with timestamp to `logs/access.log` file.
- Basic structure for building a web server.

## Prerequisites

Before you begin, ensure you have met the following requirements:

- This program is written in C and requires a C compiler (e.g., GCC) to build.
- Ensure you have the necessary privileges to listen on port 80 (usually requires superuser/root privileges).

## How to Compile

To compile the program, follow these steps:

1. Clone or download this repository to your local machine.

2. Open a terminal and navigate to the project directory:

```bash
   cd /path/to/project_directory
```
3. Compile the program using the provided Makefile:
```bash
   make
```
This will create the tcp_listen executable in the bin directory.

## How to Run

To run the web server, use the following command:

```bash
   ./bin/tcp_listen
```
The server will start listening on port 80. You may need superuser/root privileges to bind to port 80.

## Usage
1. The server will listen on port 80 for incoming connections.
2. Access logs will be written to the logs/access.log file in the project directory.
3. For a complete web server, you can extend the program to handle HTTP requests, serve web pages, and implement more advanced features.
