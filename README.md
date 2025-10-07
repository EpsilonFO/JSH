# JSH - Job Shell

## Overview

JSH (Job Shell) is a Unix shell implementation written in C that provides core shell functionality including command execution, job control, pipelines, I/O redirection, and command substitution. This project demonstrates fundamental concepts of process management, inter-process communication, and signal handling in Unix-like operating systems.

## Project Description

JSH is a functional command-line interpreter that mimics the behavior of traditional Unix shells like bash or zsh. It was developed as a systems programming project to understand the inner workings of shell implementations.

### Key Features

#### Core Shell Functionality
- **Command Execution**: Execute external commands using `execvp`
- **Built-in Commands**: Native implementation of common shell commands
- **Command History**: Integration with GNU Readline for command history and line editing
- **Environment Variables**: Support for environment variable operations

#### Job Control
- **Background Jobs**: Execute commands in the background using `&`
- **Job Management**: Track, list, and control multiple background processes
- **Job Status Tracking**: Monitor job states (Running, Stopped, Done, Killed)
- **Foreground/Background Switching**: Move jobs between foreground and background

#### Advanced Features
- **Pipelines**: Chain multiple commands using `|` operator
- **I/O Redirection**: 
  - Input redirection (`<`)
  - Output redirection (`>`, `>>`, `>|`)
  - Error redirection (`2>`, `2>>`, `2>|`)
- **Command Substitution**: Execute commands and use their output as input (`<(command)`)
- **Signal Handling**: Proper signal management for job control (SIGINT, SIGTSTP, SIGCHLD, etc.)

### Built-in Commands

- `cd [directory]`: Change the current directory
  - `cd ~`: Go to home directory
  - `cd -`: Go to previous directory
  - `cd /path`: Go to absolute path
- `pwd`: Print the current working directory
- `exit [code]`: Exit the shell with optional exit code
- `jobs`: List all background jobs with their status
- `fg %job`: Bring a background job to the foreground
- `bg %job`: Resume a stopped job in the background
- `kill [-signal] %job|pid`: Send a signal to a job or process
- `?`: Display the exit status of the last command

## Architecture

### File Structure

#### Main Components
- `jsh.c`: Main shell loop, signal configuration, and initialization
- `main.c`: Entry point of the application
- `prompt.c`: Prompt generation and display management
- `commandes.c`: Command dispatcher and built-in command router

#### Command Execution
- `extern_managment.c`: External command execution management
- `functions.c`: Core execution functions and signal handlers
- `pipe.c`: Pipeline implementation and multi-command chaining
- `substitutions.c`: Command substitution processing

#### Built-in Commands
- `cd.c`: Directory navigation implementation
- `exit.c`: Shell exit handling
- `bg.c`: Background job management
- `fg.c`: Foreground job management
- `kill.c`: Process signal sending

#### System Features
- `jobs.c`: Job tracking, addition, removal, and status updates
- `redirect.c`: I/O redirection handling (stdin, stdout, stderr)

#### Header Files
- `globals.h`: Global variables and shared data structures
- `jsh.h`, `cmd.h`, `functions.h`, `jobs.h`, `prompt.h`, `pipe.h`, `redirect.h`, `substitution.h`: Function declarations and type definitions

### Job Structure

```c
typedef struct jobs {
    char *name;           // Command name/string
    int num_job;          // Job number
    char *status;         // Job status (Running, Stopped, Done, Killed)
    pid_t pid;            // Process ID
    int id_group_proc;    // Process group ID
} jobs;
```

### Command Processing Flow

1. **Input Reading**: User input is read using GNU Readline
2. **Command Parsing**: Input is tokenized and analyzed for special operators
3. **Command Type Detection**: Determine if command is built-in or external
4. **Redirection Setup**: Parse and configure I/O redirections
5. **Pipeline Detection**: Identify and set up pipes if needed
6. **Execution**: 
   - Built-in commands execute directly in the shell process
   - External commands fork and execute via `execvp`
7. **Job Management**: Track background jobs and update their status
8. **Signal Handling**: Manage signals for proper job control

## Getting Started

### Prerequisites

- **C Compiler**: GCC or compatible C compiler
- **GNU Readline Library**: For command-line editing and history
  ```bash
  # On Debian/Ubuntu
  sudo apt-get install libreadline-dev
  
  # On macOS
  brew install readline
  ```
- **Make**: For building the project
- **POSIX-compliant OS**: Linux, macOS, or other Unix-like system

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/EpsilonOF/JSH.git
   cd JSH
   ```

2. Compile the project:
   ```bash
   make
   ```
   This will generate the `jsh` executable.

3. (Optional) Clean build artifacts:
   ```bash
   make clean
   ```

### Running JSH

Launch the shell:
```bash
./jsh
```

You will see a prompt displaying the number of background jobs and current directory:
```
[0]/path/to/directory$ 
```

## Usage Examples

### Basic Commands
```bash
[0]/home/user$ ls -l
[0]/home/user$ pwd
/home/user
[0]/home/user$ cd Documents
[0]/home/user/Documents$ cd -
/home/user
```

### Background Jobs
```bash
[0]/home/user$ sleep 100 &
[1]   12345        Running sleep 100
[1]/home/user$ jobs
[1] 12345  Running  sleep 100
[1]/home/user$ fg %1
# Job moves to foreground
```

### Job Control with Signals
```bash
[0]/home/user$ sleep 100
^Z  # Press Ctrl+Z to stop
[1]   12345        Stopped sleep 100
[1]/home/user$ bg %1
[1]   12345        Running sleep 100
[1]/home/user$ kill %1
```

### Pipelines
```bash
[0]/home/user$ ls -l | grep ".c" | wc -l
[0]/home/user$ cat file.txt | sort | uniq
```

### I/O Redirection
```bash
# Output redirection
[0]/home/user$ echo "Hello World" > output.txt
[0]/home/user$ ls -l >> output.txt

# Input redirection
[0]/home/user$ wc -l < input.txt

# Error redirection
[0]/home/user$ command_that_fails 2> errors.txt
[0]/home/user$ make 2>> build_errors.log
```

### Command Substitution
```bash
[0]/home/user$ cat <(ls -l)
[0]/home/user$ diff <(sort file1.txt) <(sort file2.txt)
```

### Exit Status
```bash
[0]/home/user$ ls /nonexistent
ls: cannot access '/nonexistent': No such file or directory
[0]/home/user$ ?
1
```

## Implementation Details

### Signal Management

The shell configures the following signal behaviors:
- **Parent Shell**: Ignores SIGINT, SIGTERM, SIGTTIN, SIGQUIT, SIGTTOU, SIGTSTP
- **Child Processes**: Restore default signal handlers
- **SIGCHLD**: Custom handler to track terminated/stopped background jobs

### Process Groups

JSH uses process groups for proper job control:
- Each job runs in its own process group
- Foreground jobs receive terminal control via `tcsetpgrp()`
- Background jobs run without terminal access

### Memory Management

The shell carefully manages memory allocation and deallocation:
- Dynamic allocation for command arguments and job structures
- Proper cleanup on exit and job completion
- History management through GNU Readline

## Testing

Run the automated test suite:
```bash
bash test.sh
```

This will clone and execute the official test suite for validation.

## Authors

- OLLIVIER Félix (22107258) - @ollivier
- ALLAL Yanis (22102871) - @allaly
- ALI MULLUD Asso (22001016) - @alimullu

## Technical Notes

### Parsing Strategy
- Uses `strtok()` for tokenization on spaces and pipe operators
- Sequential parsing approach for redirections and special operators

### Limitations
- Maximum of 100 concurrent background jobs
- No support for shell scripting constructs (if/then/else, for loops, etc.)
- No wildcard expansion (globbing)
- No alias support
- No command completion beyond Readline's default

## License

This project was developed as part of a university systems programming course.

## Acknowledgments

This project demonstrates practical application of:
- Process creation and management (`fork`, `exec`, `wait`)
- Inter-process communication (`pipe`)
- File descriptor manipulation (`dup2`, `open`, `close`)
- Signal handling (`sigaction`, `kill`)
- Terminal control (`tcsetpgrp`, `tcgetpgrp`)
