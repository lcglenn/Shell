# C++ Shell Implementation

A simple Unix/Linux shell implementation written in C++. This project demonstrates fundamental operating system concepts including process creation, command execution, and system calls.

## Overview

This shell implementation provides a command-line interface similar to bash or other Unix shells. It supports basic command execution, built-in commands, and proper error handling.

## Features

- **Command Execution**: Run external programs and commands
- **Built-in Commands**:
  - `cd`: Change directory
  - `help`: Display help information
  - `exit`: Exit the shell
- **Error Handling**: Appropriate error messages for command not found and other failures
- **Modern C++ Design**: Uses C++11 features including:
  - Smart containers (std::vector, std::string)
  - Exception handling
  - Automatic memory management

## Requirements

- C++11 compatible compiler (g++ or clang++)
- Unix/Linux-based operating system (Linux, macOS, etc.)

## Compilation

Compile the shell using:

```bash
g++ -std=c++11 -o myshell myshell.cpp
```

## Usage

Run the shell:

```bash
./myshell
```

You will be presented with a prompt (">") where you can enter commands.

### Examples

```
> ls
[directory contents will be listed]

> echo hello world
hello world

> cd /some/directory
[changes current directory]

> help
[displays built-in commands]

> exit
[exits the shell]
```

## Implementation Details

The shell works by:

1. Displaying a prompt to the user
2. Reading command input from the user
3. Parsing the input into tokens
4. Checking if the command is built-in
   - If built-in, executing the corresponding function
   - If not built-in, creating a child process to execute the command
5. Waiting for the command to complete
6. Repeating the process

Key components:
- Command parsing: Tokenizes user input
- Command execution: Handles both built-in and external commands
- Process management: Creates and manages child processes

## Future Enhancements

Potential future improvements:
- Input/output redirection
- Pipes for command chaining
- Background process execution
- Command history
- Tab completion

## Author

Liam Glenn
