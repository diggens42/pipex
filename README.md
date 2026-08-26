*This project has been created as part of the 42 curriculum by fwahl.*

# pipex

Rebuilding the shell's `|` operator from scratch, with `pipe`, `fork`, `dup2` and `execve`.

```
./pipex file1 cmd1 cmd2 file2     ≡     < file1 cmd1 | cmd2 > file2
```

---

## Description

### The goal

Every time you type `< infile ls -l | wc -l > outfile` into a shell, a surprising amount
of machinery runs: the shell creates a pipe, forks twice, rewires the standard input and
output of each child onto the right ends of that pipe and onto the right files, replaces
each child with the requested program, and finally waits for them and reports the exit
status of the last one.

`pipex` is that machinery, written by hand. The program takes four arguments — an input
file, two shell commands, and an output file — and must behave *exactly* like the
equivalent shell pipeline, down to the error messages and the exit code.

The point of the exercise is not the pipe itself, which is one system call. It is
everything around it: process creation, file descriptor inheritance, the difference
between a parent and a child after `fork`, why a pipe end left open makes a program hang
forever, and how a shell decides what number to return when something goes wrong.

### The mechanism

A pipe is a unidirectional byte buffer inside the kernel with two file descriptors
attached to it: `fd[0]` to read from, `fd[1]` to write to. On its own it is only useful
for talking to yourself. It becomes the shell's `|` when combined with two other ideas:

1. **`fork` copies the file descriptor table.** After forking, both processes hold both
   ends of the pipe. Each one closes the end it does not need.
2. **`dup2` renumbers a descriptor.** `dup2(fd[1], STDOUT_FILENO)` makes descriptor `1`
   point at the write end of the pipe. The program that gets `execve`d afterwards knows
   nothing about any of this — it writes to stdout as always, and the bytes land in the
   pipe instead of the terminal.

The reason a leftover pipe end is fatal deserves a sentence of its own: `read` on a pipe
returns `0` (EOF) only once *every* write end everywhere in the system has been closed.
If the parent forgets to close its own copy of `fd[1]`, the second command waits for an
end-of-file that can never arrive, and the whole program hangs.

### How this implementation works

```
                  ┌──────────────────── parent ────────────────────┐
                  │   pipe(fd)   →   fd[0] = read, fd[1] = write   │
                  └────────┬──────────────────────────────┬────────┘
                        fork()                         fork()
                           │                              │
          ┌────────────────▼──────────┐   ┌───────────────▼─────────────────┐
          │ child 0                   │   │ child 1                         │
          │  open(file1, O_RDONLY)    │   │  open(file2, O_WRONLY|O_CREAT   │
          │  dup2(infile,  STDIN)     │   │                     |O_TRUNC)   │
          │  dup2(fd[1],   STDOUT) ───┼───┼─▶ dup2(fd[0],  STDIN)           │
          │  execve(cmd1)             │   │  dup2(outfile, STDOUT)          │
          └───────────────────────────┘   │  execve(cmd2)                   │
                                          └─────────────────────────────────┘

                  ┌────────────────────────────────────────────────┐
                  │ parent closes every descriptor it still holds,  │
                  │ waits for both children, returns cmd2's status  │
                  └────────────────────────────────────────────────┘
```

All state lives in a single `t_px` struct (`include/pipex.h`) that is passed around by
pointer, so any function that has to bail out can close the descriptors and free the
allocations it knows about before exiting.

**`main`** initialises the struct, checks the argument count, creates the pipe, forks
twice through `ft_pipex`, then closes its own copies of the descriptors and waits.

**`ft_child_io`** does the redirection inside each child. The important detail is that
**each child opens its own file**. An earlier version opened both files in the parent
before forking, which looks tidier but is wrong: in a real shell each side of a pipeline
is a separate process performing its own redirections, so a broken redirect only kills
that one side. With `./pipex infile cat wc /root/denied`, bash still runs `cat`, and only
the `wc` side dies. Opening both files up front turned that into a fatal error for the
whole program, and the output of `cmd1` disappeared.

**`ft_exec_cmd`** splits the command string on spaces, resolves the binary, and calls
`execve`. Resolution mirrors what a shell does: a command containing a `/` is used as a
path verbatim, otherwise `PATH` is read out of `envp`, split on `:`, and each directory
is tried in order until one yields an executable file. If `envp` carries no `PATH` at
all, the command is only tried as a literal path.

**`wait_childs`** reaps both children and returns the exit status of the *second* one,
because the exit status of `a | b` is the exit status of `b`. If that child was killed by
a signal instead of exiting normally, the return value is `128 + signal`, which is again
the shell's convention (`kill -9` shows up as `137`).

### Error handling and exit codes

The subject's rule is "if you are unsure, handle errors the same way as the shell", so
the exit codes follow bash:

| Situation | stderr | exit code |
| --- | --- | --- |
| Wrong number of arguments | usage message | `1` |
| Input file missing or unreadable | `perror` | status of `cmd2` (the pipeline continues) |
| Output file cannot be created | `perror` | status of `cmd2` |
| Command not found in `PATH` | `pipex: cmd: command not found` | `127` |
| Found but not executable | `pipex: cmd: permission denied` | `126` |
| Command killed by a signal | — | `128 + signal` |
| Everything worked | — | status of `cmd2` |

Diagnostics always go to `STDERR_FILENO`. Writing them to stdout would be a real bug and
not just a cosmetic one, since in a pipeline stdout is the next command's input.

Memory and descriptors are released on every exit path, including inside the children
just before `execve`, and the program is clean under
`valgrind --leak-check=full --track-fds=yes`.

### Project structure

```
include/pipex.h     the t_px struct and all prototypes
src/main.c          argument check, pipe creation, entry point
src/pipex.c         forking, child redirection, execve, waiting
src/file.c          PATH lookup and opening the two files
src/utils.c         cleanup and the error exits
libft/              my own C library, pulled in as a git submodule
```

### Scope

Only the mandatory part is implemented: exactly two commands, one input file, one output
file. The bonus (arbitrarily many pipes, and `here_doc` with `<<` / `>>`) is **not** part
of this repository.

---

## Instructions

### Requirements

A Linux or macOS environment with `cc` and `make`. `libft` is a git submodule, so clone
recursively:

```bash
git clone --recurse-submodules <repo-url> pipex
cd pipex
```

If you already cloned without `--recurse-submodules`, it still works — the `Makefile`
runs `git submodule init` and `git submodule update` before building the library.

### Build

```bash
make          # build libft, then pipex
make clean    # remove object files (and the tester, if it was cloned)
make fclean   # clean, plus the binary
make re       # fclean, then all
make test     # fetch and run the test suite (see Testing)
make testclean # remove the cloned tester only
```

The compiler is `cc` with `-Wall -Wextra -Werror`, and the `Makefile` does not relink
unnecessarily.

### Usage

```bash
./pipex file1 "cmd1" "cmd2" file2
```

Each command must be passed as a **single quoted argument**, so that its own flags stay
attached to it. `file2` is created if it does not exist and truncated if it does.

### Examples

```bash
./pipex infile "ls -l" "wc -l" outfile        # < infile ls -l | wc -l > outfile
./pipex infile "grep a1" "wc -w" outfile      # < infile grep a1 | wc -w > outfile
./pipex infile "cat" "sort -r" outfile        # < infile cat | sort -r > outfile
./pipex /etc/passwd "cut -d: -f1" "sort" out  # < /etc/passwd cut -d: -f1 | sort > out
```

The easiest way to check any of these is to run the shell line next to it and `diff` the
two output files, which is exactly what the tester below does.

### Testing

I wrote my own test suite, which lives in a separate repository:
**[pipex_tester](https://github.com/diggens42/pipex_tester)**. The `Makefile` knows how to
fetch and run it:

```bash
make test
```

That clones the tester on first use, pulls it on later runs, and executes all 55 cases
against the freshly built binary. It is **not** cloned by a plain `make` — it is shell
code rather than C, it is not part of the submitted source, and nothing in the mandatory
build depends on it. `make clean` and `make fclean` remove it again, so a full clean
always leaves the repository exactly as it was cloned.

The idea behind it is that no expected output is written down anywhere. Every case is run
twice — once through `pipex`, and once through the real `< file1 cmd1 | cmd2 > file2`
pipeline in bash — and the shell is the reference. Five things are then compared:

| Column | What it checks |
| --- | --- |
| `out` | the output file, byte for byte |
| `exit` | the exit code of the whole pipeline |
| `err` | that a diagnostic was printed, and that it went to stderr |
| `time` | wall-clock duration, which catches a parent that does not wait for its children |
| `mem` | valgrind, with leaks and unclosed descriptors tracked across forks |

The 55 cases are grouped into basic pipelines, file errors, command errors, invocation,
PATH handling, memory and descriptors, and waiting. The suite needs nothing beyond a
normal Linux or macOS shell; if `valgrind` is not installed it says so and skips the nine
memory cases instead of failing them, so it still gives a useful result on any machine.

The `mem` column is the part that took the most work. Every other pipex tester I tried
reported leaks for a correct program, because `ls`, `cat` and `wc` do not free their own
allocations before exiting and valgrind counts those against you. Restricting the report
to `--show-leak-kinds=definite,indirect` drops that noise entirely, and I verified the
check still fires by pointing it at a program that deliberately leaks a descriptor and a
buffer. Every run is also wrapped in a `timeout`, so a forgotten pipe close is reported
as a hang instead of freezing the suite.

If you would rather run it by hand, or point it at someone else's binary during a peer
evaluation:

```bash
git clone https://github.com/diggens42/pipex_tester.git
bash pipex_tester/tester.sh ./pipex
```

---

## Resources

### My brain

Most of the time here went into thinking rather than reading: drawing the two processes
and their descriptor tables on paper until it was obvious which end had to be closed
where, and reasoning through why the program hung before adding any code to fix it. The
structure of the project, the `t_px` struct and the split across the four source files
are my own.

### The manual

The man pages are the actual specification for this project, and nearly every question I
had was answered by one of them:

```bash
man 7 pipe        # overview of pipes and FIFOs, and the EOF rules
man 2 pipe        # creating the pipe
man 2 fork        # what a child inherits
man 2 dup2        # redirecting a file descriptor
man 2 execve      # replacing the process image, and why it never returns
man 2 waitpid     # reaping children, WIFEXITED / WEXITSTATUS / WTERMSIG
man 2 open        # O_CREAT, O_TRUNC and the mode argument
man 2 access      # F_OK vs X_OK
man 3 perror      # error reporting the way the subject asks for
```

`man 7 pipe` is the one worth reading start to finish — it is where the "read returns 0
only when all write ends are closed" rule is actually written down.

### Articles and videos

- [Pipe: an Inter-Process Communication Method](https://www.codequoi.com/en/pipe-an-inter-process-communication-method/) — Mia Combeau. The clearest write-up I found on combining `pipe` with `fork`, with diagrams of the descriptor table at each step.
- [Unix Processes in C](https://www.youtube.com/playlist?list=PLfqABt5AS4FkW5mOn2Tn9ZZLLDwA3kZUY) — CodeVault. A playlist covering `fork`, `wait`, pipes and FIFOs; the episodes on simulating the `|` operator map almost directly onto this project.
- [Beej's Guide to Unix IPC](https://beej.us/guide/bgipc/), chapter 5 — short, old, and still the friendliest introduction to why pipes work the way they do.
- *The Linux Programming Interface*, Michael Kerrisk, chapter 44 — the reference I reached for when the man pages were too terse, especially on pipe capacity and blocking behaviour.

### AI

I used AI (Claude) for two things, both towards the end of the project:

**Finding bugs and edge cases.** Once the program was written and working on the obvious
inputs, I used it as a reviewer to attack the cases I had not thought of. The most
valuable find was the design flaw described above — opening both files in the parent
before forking, which made a bad output file kill the entire pipeline instead of just the
second command. It also caught a few smaller things: an empty command string like `""`
reaching `execve`, an error path that printed with `perror` when it should have exited
with `127`, and a string that was being added to an integer instead of concatenated. I
also used it to help build the tester repository linked above, in particular to work out
why valgrind reported leaks in `ls` and `cat` (they were "still reachable" blocks from
the commands themselves, not from my code) and how to filter those out so the leak column
means something.

Every fix it suggested I wrote myself, so that I can explain each one during evaluation.

**Writing this README**, because I am lazy. The content is mine — the architecture, the
reasoning about the bug, and the resource list all come from the work above — but the
prose was drafted by AI from the source files and the subject PDF, and then reviewed and
corrected by me.
