# C-Shell

This project is a custom shell implemented in C, designed to mimic the functionality of a Unix shell. The shell supports a range of features, including command execution, background processes, input/output redirection, and piping. It also handles various Unix signals like `Ctrl+C` and `Ctrl+Z` to provide a robust command-line environment.

## Key Files

- **`main.c`**: The entry point of the shell, responsible for initializing the shell and processing user input.
- **`execute.c`**: Contains the logic for executing commands, both built-in and system commands.
- **`background.c`**: Manages background process execution and tracking.
- **`in_out_reduction.c`**: Handles the redirection of input and output streams.
- **`pipes.c`**: Implements the piping mechanism to allow output from one command to serve as input to another.
- **`signal.c`**: Deals with the handling of signals like interrupts and job control.

## Project Structure

The project is structured to be modular, with each C file responsible for a specific functionality, making it easy to maintain and extend.

