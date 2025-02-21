# Pipex

## Description
Pipex is a project that mimics the behavior of shell pipes. It allows chaining commands together using pipes, redirecting input and output as specified. The project takes an input file, processes it through two commands, and outputs the result to a specified output file.

## Features
- Executes two commands connected via a pipe.
- Redirects input from a file and output to another file.
- Handles path resolution for commands.
- Proper error handling for system calls.

## Usage
### Compilation
Compile the program using:
```sh
make
```

Compile the bonus using:
```sh
make bonus
```

### Running Pipex
Run the executable with the following syntax:
```sh
./pipex file1 cmd1 cmd2 file2
```

Run the bonus with the following syntax:
```sh
./pipex file1 cmd1 cmd2 ... cmdn file2
```

Where:
- `file1`: Input file.
- `cmd1`: First command to execute.
- `cmd2`: Second command to execute.
- `file2`: Output file.

### Example
```sh
./pipex infile "ls -l" "wc -l" outfile
```
This will:
1. Read data from `infile`.
2. Execute `ls -l` and pass its output to `wc -l`.
3. Save the final result in `outfile`.

## Error Handling
- Displays appropriate error messages for failures in system calls (`pipe`, `fork`, `dup2`, `execve`, etc.).
- Prints "Error: command not found" if a command cannot be executed.

## Functions Used
- `pipe()`
- `fork()`
- `dup2()`
- `execve()`
- `waitpid()`
- File handling functions (`open`, `close`, `access`)

### Project has been evaluated with 100% score, but bonus hasn`t been submited!!
