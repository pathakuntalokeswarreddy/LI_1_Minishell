# MiniShell in C

A simple Linux shell implemented in C that accepts user commands and executes them using Linux system calls.

## Features

* **Interactive Shell:** Provides an interactive command-line interface with user-friendly input handling.
* **Custom Prompt:** Allows users to change the shell name using `PS1=<name>$`.
* **Built-in Commands:** Supports internal commands such as `echo`, `fg`, `bg`, `jobs`, and `cd`.
* **External Commands:** Executes external commands along with options.
* **Pipeline Execution:** Supports executing multiple commands connected through pipes (`|`).
* **Signal Handling:** Implements signal handling to manage process behavior and user interruptions.

## Technologies Used

* C Programming
* Linux System Calls
* Process Management (`fork()`, `exec()`, `waitpid()`)
* Inter-Process Communication (Pipes)
* Makefile
* GCC Compiler

## Requirements

* Linux operating system
* GCC compiler
* GNU Make


## Learning Outcomes

* Understanding Linux process creation and execution.
* Working with system calls and file descriptors.
* Implementing command parsing and execution.
* Understanding inter-process communication using pipes.
* Managing processes using `fork()`, `exec()`, and `waitpid()`.

---

**Note:** This project is developed for learning Linux system programming and shell implementation in C.
