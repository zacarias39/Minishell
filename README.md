*This project has been created as part of the 42 curriculum by zcasimir, dadmendo.*

# Minishell

A simple shell implementation in C, recreating basic functionality of bash.

![Shell](https://img.shields.io/badge/Shell-Minishell-blue)
![Language](https://img.shields.io/badge/Language-C-orange)
![School](https://img.shields.io/badge/School-42-green)

## Description

Minishell is a project that challenges students to create their own shell, similar to bash. The goal is to understand how a shell works by implementing:

- **Command execution** - Running external programs using `execve`
- **Built-in commands** - `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- **Redirections** - Input (`<`), output (`>`), append (`>>`), and heredoc (`<<`)
- **Pipes** - Connecting commands with `|`
- **Environment variables** - Expansion of `$VAR` and `$?`
- **Signal handling** - Proper handling of `ctrl-C`, `ctrl-D`, and `ctrl-\`
- **Quote handling** - Single (`'`) and double (`"`) quotes interpretation

This project provides deep insight into process management, file descriptors, and UNIX system calls.

## Instructions

### Prerequisites

- GCC compiler
- GNU Make
- Readline library

On Debian/Ubuntu:
```bash
sudo apt-get install libreadline-dev
```

### Compilation

Clone the repository and compile:

```bash
git clone <repository-url> minishell
cd minishell
make
```

Available Make rules:
- `make` or `make all` - Compile the project
- `make clean` - Remove object files
- `make fclean` - Remove object files and executable
- `make re` - Recompile the project

### Execution

```bash
./minishell
```

## Usage Examples

```bash
minishell$ echo "Hello, World!"
Hello, World!

minishell$ ls -la | grep minishell | wc -l
2

minishell$ cat < input.txt > output.txt

minishell$ export MY_VAR="42"
minishell$ echo $MY_VAR
42

minishell$ cat << EOF
> This is a heredoc
> EOF
This is a heredoc

minishell$ exit
```

## Features

| Feature | Status |
|---------|--------|
| Simple command execution | ✅ |
| Command with arguments | ✅ |
| Built-in: echo (with -n) | ✅ |
| Built-in: cd | ✅ |
| Built-in: pwd | ✅ |
| Built-in: export | ✅ |
| Built-in: unset | ✅ |
| Built-in: env | ✅ |
| Built-in: exit | ✅ |
| Input redirection (`<`) | ✅ |
| Output redirection (`>`) | ✅ |
| Append redirection (`>>`) | ✅ |
| Heredoc (`<<`) | ✅ |
| Pipes (`\|`) | ✅ |
| Environment variables | ✅ |
| Exit status (`$?`) | ✅ |
| Single quotes | ✅ |
| Double quotes | ✅ |
| Signal handling | ✅ |

## Project Structure

```
minishell/
├── includes/           # Header files
│   ├── minishell.h
│   ├── lexer.h
│   ├── parser.h
│   └── expansion.h
├── src/
│   ├── builtin/        # Built-in commands implementation
│   ├── env/            # Environment management
│   ├── execute/        # Command execution logic
│   ├── lexer/          # Tokenization
│   ├── parser/         # AST construction
│   ├── signals/        # Signal handlers
│   └── utils/          # Utility functions
├── libft/              # Custom C library
├── Makefile
└── README.md
```

## Resources

### Documentation
- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- [Readline Library Documentation](https://tiswww.case.edu/php/chet/readline/rltop.html)

### System Calls
- `man 2 fork` - Process creation
- `man 2 execve` - Program execution
- `man 2 pipe` - Inter-process communication
- `man 2 dup2` - File descriptor duplication
- `man 2 wait` - Process synchronization
- `man 7 signal` - Signal handling

### Tutorials
- [Writing a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/)
- [Shell Implementation](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)

### AI Usage

AI tools (GitHub Copilot) were used in this project for:
- **Documentation**: Generating this README structure and content
- **Code review**: Identifying potential memory leaks and edge cases
- **Debugging assistance**: Understanding segmentation faults and redirect issues

All core logic, parsing algorithms, and execution flow were implemented manually by the team members.

## Authors

- **zcasimir** - [42 Intra Profile](https://profile.intra.42.fr/users/zcasimir)
- **dadmendo** - [42 Intra Profile](https://profile.intra.42.fr/users/dadmendo)

## License

This project is part of the 42 School curriculum. Feel free to use it as a reference, but please don't copy it directly for your own submission.