# LiteShell - A Simple Shell Implementation

## Overview
ShellEx is a simple Unix shell implemented in C, designed for educational purposes based on concepts from the book **Computer Systems: A Programmer's Perspective (CS:APP)**. This shell supports built-in commands and executes system commands via the `execvp` system call.

## Features
- Custom shell prompt support
- Execution of both built-in and system commands
- Signal handling for process control
- Basic error handling and informative feedback

## Installation
### Prerequisites
- A Unix-based operating system (Linux/macOS)
- GCC compiler

### Steps to Compile
```sh
make
```

This will generate an executable `sh257`.

## Running the Shell
To start the shell with a custom prompt:
```sh
./sh257 -p <your_prompt>
```
Example:
```sh
./sh257 -p myShell>
```
If no custom prompt is provided, the default `sh257>` prompt is used.

## Built-in Commands
ShellEx supports the following built-in commands:

| Command      | Description |
|-------------|-------------|
| `exit`      | Terminates the shell |
| `pid`       | Prints the process ID of the shell |
| `ppid`      | Prints the parent process ID |
| `help`      | Displays a help menu |
| `cd`        | Prints current directory; `cd <path>` changes the working directory |

## Functionality of Methods in `shellex.c`

### `eval(char *cmdline)`
Evaluates the command input, parses it, and determines whether it is a built-in command or should be executed via `execvp`.

### `parseline(char *buf, char **argv)`
Parses the command line into arguments, identifies if the command should run in the background.

### `builtin_command(char **argv)`
Checks if the entered command is a built-in command and executes it if so.

### `return_pid()` & `return_ppid()`
Return the process ID and parent process ID of the shell, respectively.

### `cd_built()` & `changedir(char* newpath)`
Retrieves or changes the current working directory.

### `exit_built()`
Terminates the shell process.

### `help_in()`
Displays information about supported commands.

### `sigint_handler(int sig)`
Handles `SIGINT` (Ctrl+C) to prevent the shell from terminating unintentionally.

## `Sample Run`
Below is a sample session demonstrating the primary functionalities of LiteShell:\

```sh
sh257> help
***************************************
A custom Shell for CMSC 257 by Jacob Turner
Usage:
- After using make, ./sh257 -p <prompt> will allow you to change the prompt
***************************************
BUILTIN COMMANDS:
exit: Exits the shell
pid: Prints the process ID
ppid: Prints the parent process ID
help: Displays this menu
cd: Prints current working directory while "cd <path>" changes the current working directory
SYSTEM COMMANDS:
- Use man pages for system commands

sh257> pid
12345 

sh257> cd /home/user
sh257> pwd
/home/user

sh257> ls
file1.txt  file2.txt  directory

sh257> exit
```

## Acknowledgment
This project utilizes concepts from **Computer Systems: A Programmer’s Perspective (CS:APP)** by *Randal E. Bryant and David R. O’Hallaron*. Some utility functions come from the `csapp.c` and `csapp.h` files associated with this book.

## License
This project is for educational purposes and follows fair-use practices for learning from CS:APP.

