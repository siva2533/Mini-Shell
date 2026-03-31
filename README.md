# 🐚 Mini Shell
A lightweight Unix shell implementation built from scratch in C that replicates core functionalities of a standard Bash shell — including command parsing, execution, piping, signal handling, and full job control.
---
## ✨ Features
| Feature | Description |
|---------|-------------|
| **Built-in Commands** | `cd`, `pwd`, `echo`, `exit`, `printf`, `fg`, `bg`, `jobs` and more |
| **External Commands** | Supports 150+ external Linux commands (`ls`, `grep`, `cat`, `wc`, etc.) |
| **Piping** | Multi-stage pipe support (e.g., `ls \| grep .c \| wc -l`) |
| **Signal Handling** | Custom handlers for `Ctrl+C` (SIGINT) and `Ctrl+Z` (SIGTSTP) |
| **Job Control** | Stop, resume (foreground/background), and list jobs with status tracking |
| **Custom Prompt** | Change the shell prompt dynamically using `PS1=<new_prompt>` |
---
## 🏗️ Project Structure
```
Mini-Shell/
├── main.c              # Entry point — initializes shell and starts input loop
├── scan.c              # Input scanning, command dispatch, and signal handling
├── checkcmd.c          # Command classification (built-in vs external) & external command loading
├── exec_int.c          # Execution logic for built-in/internal commands
├── exec_ext.c          # Execution logic for external commands with pipe support
├── builtin.c           # List of all supported built-in commands
├── list_operation.c    # Linked list operations for job control (insert, delete, update status)
├── header.h            # Header file with includes, macros, structs, and function prototypes
└── ext_cmd.txt         # List of supported external commands
```
---
## 🔧 How It Works
### Command Flow
```
User Input → Parse Command → Classify (Built-in / External / Unknown)
                                  │
                  ┌───────────────┼───────────────┐
                  ▼               ▼               ▼
            Built-in         External         "command
          (exec_int.c)     (exec_ext.c)      not found"
                              │
                        ┌─────┴─────┐
                        ▼           ▼
                    No Pipe      Has Pipes
                   (execvp)    (pipe + fork
                                + dup2)
```
### Job Control (Bash-like Behavior)
The shell supports real Bash-style job control with **Stopped** and **Running** status tracking:
```bash
minishell$: sleep 50
^Z
[1]+  Stopped			sleep 50
minishell$: jobs
[1]+  Stopped			sleep 50
minishell$: bg
[1]+ sleep 50 &
minishell$: jobs
[1]-  Running			sleep 50 &
minishell$: fg
sleep 50
```
**How it works internally:**
- Jobs are stored in a **linked list**, each node tracking `PID`, `job number`, `status` (Stopped/Running), and the `command string`
- `Ctrl+Z` sends `SIGTSTP` → process is stopped and added to the job list
- `bg` sends `SIGCONT` → updates job status to **Running**
- `fg` sends `SIGCONT` → waits for the process in the **foreground**
- `SIGCHLD` handler cleans up finished background jobs automatically
### Signal Handling
| Signal | Trigger | Shell Behavior |
|--------|---------|----------------|
| `SIGINT` | `Ctrl+C` | Kills foreground process; shell itself is protected |
| `SIGTSTP` | `Ctrl+Z` | Stops foreground process and adds it to the job list |
| `SIGCHLD` | Child exits | Automatically removes completed background jobs |
---
## 🚀 Getting Started
### Prerequisites
- GCC compiler
- Linux/Unix environment (or WSL on Windows)
### Compile & Run
```bash
gcc -o minishell main.c scan.c checkcmd.c exec_int.c exec_ext.c builtin.c list_operation.c
./minishell
```
---
## 💡 Usage Examples
### Basic Commands
```bash
minishell$: ls -la
minishell$: pwd
minishell$: cd /home
minishell$: echo Hello World
minishell$: exit
```
### Special Echo Variables
```bash
minishell$: echo $$              # prints shell PID
minishell$: echo $?              # prints last command's exit status
minishell$: echo $SHELL          # prints default shell path
```
### Piped Commands
```bash
minishell$: ls | grep .c | wc -l
minishell$: cat file.txt | sort | uniq
```
### Custom Prompt
```bash
minishell$: PS1=myshell>
myshell> ls
```
### Job Control
```bash
minishell$: sleep 100            # start a long-running process
^Z                               # Ctrl+Z to stop it
[1]+  Stopped			sleep 100
minishell$: jobs                 # list all jobs with status
[1]+  Stopped			sleep 100
minishell$: bg                   # resume in background
[1]+ sleep 100 &
minishell$: jobs                 # verify it's running
[1]-  Running			sleep 100 &
minishell$: fg                   # bring back to foreground
sleep 100
```
---
## 🛠️ Technologies Used
| Category | Details |
|----------|---------|
| **Language** | C Programming |
| **Data Structures** | Linked Lists (job control with status tracking) |
| **Process Management** | `fork()`, `execvp()`, `waitpid()` |
| **Signal Handling** | `SIGINT`, `SIGTSTP`, `SIGCHLD` |
| **IPC** | `pipe()`, `dup2()` |
| **Other** | File I/O, Dynamic Memory Allocation, Modular Programming |
---
## 📚 Key Challenges & Learnings
- Built a command classification system to distinguish between **built-in** and **external** commands using a lookup-based approach
- Implemented a **linked list–based job control system** with status tracking (Stopped/Running) that mirrors real Bash behavior
- Handled inter-process communication using **Unix pipes** (`pipe`, `dup2`) for multi-stage piped commands
- Implemented custom **signal handlers** for `SIGINT`, `SIGTSTP`, and `SIGCHLD` to manage foreground/background process lifecycle
- Managed process synchronization using `waitpid` with `WUNTRACED` and `WNOHANG` flags for proper job state tracking
- Overcame challenges in **pointer manipulation** and dynamic memory management for the job list
- Gained deeper understanding of the **Unix process model**, file descriptors, and shell internals
