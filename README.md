# minishell

A Unix shell written in C at 42 Berlin: a hand-written tokenizer, a small AST of `exec`, `pipe` and `redirection` nodes, and a fork and exec executor.

## Usage

Build with the default target, which builds `libft/libft.a` first and then links the binary:

```sh
make
```

Run it:

```sh
./minishell
```

A short session:

```
$ ./minishell
minishell> echo hello world
hello world
minishell> pwd
/home/user
minishell> echo hello | cat
hello
minishell> ls > out.txt
minishell> cat out.txt
Makefile
README.md
env.c
execution
headers
...
minishell> cat << EOF
heredoc> one
heredoc> two
heredoc> EOF
one
two
minishell> exit
exit
```

Notes on the build:

- The Makefile compiles with `clang` and links `-lreadline`, so the clang toolchain and the readline headers are required.
- With clang 14.0.0, `make` stops at `parsing/tokenization/scanner.c`, because `-Werror` turns the `char` against `EOF` comparison in `scanner_has_next` into an error. Adding the warning back completes the build:

```sh
make CFLAGS="-Wall -Wextra -Werror -Wno-tautological-constant-out-of-range-compare -g -I./libft"
```

## Implemented behaviour

The status column comes from the source files listed in the last column and from a build of the current `main` branch (clang 14.0.0, Ubuntu 22.04, September 2026).

| Feature | Status | Where it lives and what it does |
| --- | --- | --- |
| Pipes (`\|`) | implemented | `exec_pipe` in `execution/exec.c` creates a pipe, forks twice, closes both ends in the parent, and waits for both children. Chains work, for example `echo a \| cat \| wc -l`. A builtin in a pipeline runs inside the pipeline child, which is why `echo hello \| cat` works even though redirected builtin output at the prompt does not. |
| Redirections (`<`, `>`, `>>`) | partial | `parse_redir` in `parsing/redirection/redirections.c` builds an `N_REDIR` node with the open flags from `get_redir_flags`; `exec_redir` in `execution/exec.c` opens the target, duplicates it onto fd 0 or fd 1, runs the command, and restores the saved descriptors. Input, output and append work for external commands (`ls > out.txt`, `ls >> out.txt`, `wc -l < a.txt`). Output from a builtin stays on the terminal instead of the file. Two redirections in one command, or a redirection combined with a pipe, crash the shell. |
| Quoting | partial | Single quotes produce one literal argument with no expansion (`single_quote_token` in `parsing/tokenization/token.c`). Double quotes are not a grouping construct: `"` is part of `WHITESPACES` in `headers/char_itr.h` and acts as a separator, so `echo "a b c"` yields three arguments, although `$VAR` inside them still expands (`double_quote_token` and `expand_env_var` in `parsing/tokenization/token.c`). An unclosed single quote asks for more input with the prompt `quote> `. |
| Expansions | partial | `expand_env_var` in `parsing/tokenization/token.c` and `expand_env_var_value` in `env.c` handle `$VAR`, `$$` (the shell pid) and `$?`. An unknown variable expands to an empty string. `$?` always reports 0. Lookup is prefix based (`ms_get_env` in `env.c`), so a name that is a prefix of an inherited variable can resolve to the wrong entry. |
| Heredocs (`<<`) | partial | `collect_heredoc_input` in `parsing/redirection/heredoc.c` reads lines at parse time with the prompt `heredoc> `; `handle_heredoc` in `execution/exec.c` then writes that text into a pipe used as the command's stdin. The body is not expanded, and the delimiter is compared literally. Ctrl-C does not cancel a heredoc. |
| Wildcards | not implemented | There is no glob step. `*` matches no token rule, so `scanner_peek` in `parsing/tokenization/scanner.c` sends it to `tmp_unknown_token` in `parsing/tokenization/token.c`, which recurses until the shell dies. |
| Builtins | partial | `builtins/init/init_builtin.c` builds the list, `is_builtin` and `execute_builtin` in `builtins/execute.c` dispatch on `argv[0]`. `echo` (with `-n`), `cd` (updates `PWD` and `OLDPWD`), `pwd`, `export` (prints `declare -x ...` without arguments), `env` and `exit` (numeric validation, status 255 on a non-numeric argument) work. `unset` prints the environment instead of removing a variable. |
| Signal handling | partial | `signal_handler.c` holds an interactive SIGINT handler and ignores SIGQUIT, but `setup_global_signals` is only called from the commented-out `main` in `main.c`, and `restore_global_signals` is never called. Only the heredoc path installs handlers, through `collect_heredoc_input`, and Ctrl-C there prints `Minishell> ` without ending the heredoc. |

## Architecture

One input line flows through lexer, parser and executor. The environment, the pipe descriptors for the current node and the last exit status live in one `t_context`, defined in `headers/env_var.h`.

- Entry point: `main.c`. `main` builds the context (`init_context`) and the builtin list (`create_builtin_list`); `shell_loop` reads a line with `readline`, adds it to the history, wraps it in a `t_char_itr`, creates the scanner, calls `parse_tree_node`, runs `exec` on the tree, and frees the tree with `free_tree_node`.
- Lexer: `parsing/tokenization/`. `parsing/tokenization/char_itr.c` is the cursor over the input line. `parsing/tokenization/scanner.c` (`scanner_next`, `scanner_peek`) looks at the first character and picks a token rule. `parsing/tokenization/token.c` holds those rules (`word_token`, `single_quote_token`, `double_quote_token`, `env_var_token`, `pipe_token`, `redir_in_token`, `redir_out_token`, `append_out_token`, `heredoc_token`) and the expansion helpers (`get_env_vvalue`, `expand_env_var`, `handle_expansions`). `parsing/tokenization/cmd.c` (`check_cmd`) tags the six builtin names as `COMMAND` and the rest as `WORD`; execution does not use that tag, it compares `argv[0]` with `is_builtin`. `parsing/tokenization/guards.c` holds `OOM_GUARD`.
- Parser: `parsing/node_creation/`. `parsing/node_creation/tree_node.c` builds the AST (`parse_tree_node`, `parse_exec`, `parse_pipe`, `visit_node`, `free_tree_node`); `parsing/node_creation/args.c` (`args_collector`, `copy_string_array`) grows the argument array word by word. There are three node kinds, `N_EXEC`, `N_PIPE` and `N_REDIR`, sharing one union (`headers/tree_node.h`).
- Redirections and heredocs: `parsing/redirection/redirections.c` (`parse_redir`, `check_redir`, `determine_target_type`, `get_redir_flags`) turns a redirection token and its target into an `N_REDIR` node. `parsing/redirection/heredoc.c` collects the heredoc body during parsing and holds the heredoc cleanup helpers. `parsing/redirection/heredoc_signals.c` is compiled from the Makefile but contains only commented-out code; the active helpers are in `signal_handler.c`.
- Executor: `execution/exec.c`. `exec` resets the context descriptors, `exec_node` dispatches on the node type, and the top level waits for the forked children. `exec_command` runs a builtin in the shell process or forks and calls `execvp`, then returns `WEXITSTATUS`. `exec_pipe` wires two children to one pipe. `exec_redir` saves and restores fd 0 and fd 1 around the redirection. `handle_heredoc` pushes the stored body through a pipe. `cleanup` frees the tree, clears the history and exits.
- Builtins: `builtins/execute.c` (dispatch), `builtins/init/init_builtin.c` (list construction), `builtins/init/init_env.c` (`check_shlvl`), `builtins/utils_bultin.c` (`find_builtin`, `ms_matrix_add_line`), and one file per builtin under `builtins/builtin/` (`builtins/builtin/ft_cd.c`, `builtins/builtin/ft_echo.c`, `builtins/builtin/ft_env.c`, `builtins/builtin/ft_exit.c`, `builtins/builtin/ft_export.c`, `builtins/builtin/ft_pwd.c`, `builtins/builtin/ft_unset.c`).
- Environment: `env.c` copies `envp` into `env` and `env_export` (`init_context`, `duplicate_env`), and provides lookup and update (`ms_get_env`, `ms_get_varenv`, `ms_set_env`, `ms_unset_env`, `find_env_index`, `ms_remove_line`, `expand_env_var_value`, `cleanup_context`).
- Signals: `signal_handler.c` (`handle_sigint`, `setup_global_signals`, `setup_heredoc_signals`, `restore_global_signals`, `is_heredoc_interrupted`, `reset_heredoc_interrupt`).
- Support: `libft/` is a vendored library that `libft/Makefile` builds; shared types and prototypes are in `headers/`.

## Testing

The project sources contain no tests, and the `Makefile` defines no test target. The repository vendors the community tester `minishell_tester/` (upstream repository LucasKuhn/minishell_tester, named in `minishell_tester/README.md`).

Run it from its own directory, with the binary present in the repository root:

```sh
cd minishell_tester
./tester
```

`minishell_tester/tester` replays every line of a suite in minishell and in bash, and compares the standard output, the exit status, and the files created in the working directory. The default run covers the lists `builtins`, `pipes`, `redirects` and `extras`. The same directory also holds the lists `wildcards`, `bonus`, `syntax`, `os_specific` and `minishell_tester/manual_tests/` (`mandatory`, `not_mandatory`, `heredoc`, `signals`); the inputs they use are in `minishell_tester/test_files/`.

The suites do not pass on the current `main` branch: the default run reports 18/146 (clang 14.0.0, Ubuntu 22.04, September 2026). The lists send arguments with `/` and operators such as `;` and `*`, which the tokenizer rejects, and they compare exit statuses and created files, which the gaps below also affect.

## Team and ownership

The repository records no team name. Git history has two committers:

- Alejandro Ramirez (`faramirezs@gmail.com`), 68 commits, under the identities `Alejandro Ramirez`, `Alejiri`, `faramirezs` and `alejiri`.
- `me11st` (`chokoboko@gmail.com`), 14 commits, 4 of them through the GitHub `users.noreply.github.com` address.

The 42 header comments in the tracked files also carry the logins `jslusark` (37 files) and `alramire` (17 files), and `lalex-ku` in the fixture `minishell_tester/test_files/loop.c`. No commit in this history is authored by `jslusark`.

## Known gaps

Each item is visible in the source or in a run of the binary.

Tokenizer:

- Any unquoted argument that contains `/` kills the shell. `scanner_peek` in `parsing/tokenization/scanner.c` has the `abs_path_token` and `rel_path_token` branches commented out, so `/` reaches `tmp_unknown_token` in `parsing/tokenization/token.c`, which calls `scanner_peek` again without moving the cursor and recurses until the stack overflows. This breaks `ls /tmp`, `cd /tmp` and `./minishell_tester/tester`.
- The same crash happens for `*`, `;`, `&&`, `~`, `\`, backticks and parentheses. They are listed in `SYMBOLS` in `headers/char_itr.h`, but no token rule handles them.
- Double quotes behave as separators instead of grouping, as described in the behaviour table. A line that ends in whitespace or in a closing quote also gains an empty argument, because `parse_tree_node` in `parsing/node_creation/tree_node.c` appends the `END` token through `args_collector` in `parsing/node_creation/args.c`.

Redirection and pipes:

- Redirected output of a builtin stays on the terminal. `exec_redir` in `execution/exec.c` restores the saved descriptors as soon as the command returns, while the builtin wrote through stdio, which buffers fully when stdout is a file. `echo hi > out.txt` creates `out.txt` at 0 bytes and prints `hi`. External commands are not affected.
- Two redirections in one command (`ls > b.txt > c.txt`) and any redirection combined with a pipe (`wc -l < a.txt | cat`) end in a segmentation fault, with no files created.
- A failed open exits the whole shell. `exec_redir` calls `cleanup` (`execution/exec.c`), which frees the tree, clears the history and exits, so `cat < missing.txt` prints `open: No such file or directory` and ends minishell with status 1.

Builtins and exit status:

- `unset` removes nothing: `handle_unset` in `builtins/builtin/ft_unset.c` calls `handle_env` once per argument, so the environment is printed instead. `ms_unset_env` in `env.c` is never called.
- `ms_get_env` in `env.c` compares only the length of the key and never checks for the `=` at that position, so a variable can resolve to an entry that merely starts with the same characters. After `export X=42`, `echo $X` prints the value of `XDG_DATA_DIRS`.
- `$?` is always 0: `exec` returns 0 (`execution/exec.c`), `shell_loop` stores that value (`main.c`), and `execute_builtin` sets `msh->ret_exit = 0` after every builtin (`builtins/execute.c`). `exit N` does propagate N as the status of the shell process.

Signals:

- The interactive loop never installs the SIGINT handler, because `setup_global_signals` is only called from the commented-out `main` in `main.c`, and `restore_global_signals` is never called. Between two heredocs the handler stays installed from the parse-time call in `collect_heredoc_input`.
- `handle_sigint` in `signal_handler.c` sets `g_heredoc_interrupt` only when the flag is already set, so Ctrl-C during a heredoc prints `Minishell> ` and the heredoc keeps reading.

Output noise and repository state:

- `exec_command` in `execution/exec.c` prints `Executing $PATH function` for every external command. An empty line forks a failed `execvp` of an empty command and prints `execvp: No such file or directory`.
- The tree still tracks build and debugging leftovers: the compiled `minishell` binary, `libft/libft.a`, the core dumps `vgcore.119296`, `vgcore.120810` and `vgcore.139432`, the NFS files `.nfs00000000106503b900000002` and `.nfs00000000106503ba00000001`, `.DS_Store`, and scratch files such as `output.txt`, `newfile.txt`, `root_processes.txt`, `sorted.txt`, `debug_output.txt`, `result.txt`, `test_input.txt`, `file.txt`, `11.txt`, `0`, `cat` and `target`. `make clean` and `make fclean` do not remove them.
