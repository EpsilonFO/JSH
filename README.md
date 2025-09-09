# JSH - Job Shell

## Overview
JSH (Job Shell) is a command-line interpreter that mimics the functionality of Unix-like shells. This project provides a functional shell environment with support for basic commands, pipelines, redirection, and various shell features.

## Features

### Core Functionality
- Command execution with arguments
- Directory navigation and file system operations
- Command history management
- Environment variable support

### Advanced Features
- I/O Redirection (`>`, `>>`, `<`)
- Command pipelines (`|`)
- Background process execution (`&`)
- Basic scripting capabilities
- Command substitution
- Wildcard expansion

### Built-in Commands
- `cd`: Change directory
- `pwd`: Print working directory
- `exit`: Exit the shell
- `echo`: Display text
- `history`: Show command history
- `jobs`: List active background processes

## Getting Started

### Prerequisites
- Java Development Kit (JDK) 8 or higher
- Git (for cloning the repository)

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/EpsilonOF/JSH.git
   ```
2. Navigate to the project directory:
   ```bash
   cd JSH
   ```
3. Compile the project:
   ```bash
   javac -d bin src/*.java
   ```
4. Run the shell:
   ```bash
   java -cp bin jsh.JSH
   ```

## Usage Examples

### Basic Command Execution
```
jsh> ls -l
jsh> pwd
jsh> cd Documents
```

### I/O Redirection
```
jsh> ls > file_list.txt
jsh> cat < input.txt
jsh> echo "Hello, World!" >> greetings.txt
```

### Command Pipelines
```
jsh> ls -l | grep ".java" | sort
jsh> cat file.txt | wc -l
```

### Background Processes
```
jsh> long_running_command &
jsh> jobs
```

## Project Structure
- `src/`: Source code files
  - `JSH.java`: Main shell implementation
  - `Command.java`: Command representation and execution
  - `Parser.java`: Input parsing and tokenization
  - `Environment.java`: Environment variable management
- `tests/`: Test scripts and test cases
- `docs/`: Documentation files

## Implementation Details

### Command Processing Flow
1. Read input from the user or script
2. Parse the input into commands, arguments, and operators
3. Expand any variables, wildcards, or command substitutions
4. Execute commands with appropriate I/O redirection
5. Manage process execution (foreground or background)
6. Return results and update shell state

### Error Handling
The shell includes robust error handling for:
- Invalid commands or syntax
- File access issues
- Permission problems
- Resource limitations

## Future Improvements
- Tab completion for commands and filenames
- More advanced scripting features (conditionals, loops)
- Custom prompt configuration
- Enhanced job control mechanisms
- Alias support

## Contributing
Contributions to improve JSH are welcome! Feel free to fork the repository, make changes, and submit pull requests.
