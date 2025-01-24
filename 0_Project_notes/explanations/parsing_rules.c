/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   parsing_rules.c									:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: jslusark <jslusark@student.42berlin.de>	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/11/17 15:43:56 by jslusark		  #+#	#+#			 */
/*   Updated: 2024/11/22 14:22:54 by jslusark		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */


/*

PLEASE READ bash_parse_explained.c BEFORE READIG THIS.

I wrote down some of the rules I am using to build my nodes from your token list.
I found out these rules while testing some input on bash, brainstorming and studyig with chat gpt,
google, git repos from other people and youtube videos we have.
Please have a read through these and test them out on your terminal,
feel free to try other similar scenarios that I haven't wrote here to see if these
rules still work! :)


	- RULE_N_1: The first token that is not part of redirection data (input symbol and filename) and that is not a pipe will be considered the command of the node.
		This rule is why all the examples below give the same result:

			echo hola hola > file1.txt
			cat file1.txt
			hola hola

			echo hola > file1.txt hola
			cat file1.txt
			hola hola

			echo > file1.txt hola hola
			cat file1.txt
			hola hola

			> file1.txt echo hola hola
			cat file1.txt
			hola hola

		This rule is also why all the examples below will give an error:

			> file2.txt hola
			bash: hello: command not found

			> file2.txt hola echo
			bash: hello: command not found

			hola > file2.txt echo
			bash: hello: command not found

			> file2.txt hola hola
			bash: hello: command not found


	- RULE_N_2: A node can start with a redirection or can also be made of just one or more redirections.
	This is why in Bash if we write this input "> file1.txt" or this input "> file1.txt > file2.txt" it is not considered an error.

		- RULE_N_2.1: If a node does not have a command, the redirection will just create the file (if it doesn't exist).
		This happens because the node does not have a command and arguments to execute on the redirection.

			EXAMPLE:
			> file2.txt
			cat file2.txt
			-creates the file but prints nothing because we don't have a command to execute on the redirection-

		- RULE_N_2.2: If a node has a command we will execute that command to its redirection.
		If the arguments are NULL we still pass them and use them to write on the file.

			EXAMPLE:
			echo > file2.txt
			cat file2.txt
			-creates the file and prints an empty file because we have the command "echo" but no args-

			EXAMPLE:
			echo hello > file2.txt
			cat file2.txt
			hello


	- RULE_N_3: The word that follows a redirection symbol (>, <, << or >>) is always considered a file or a path(for >, < and >>)
	or delimiter(for heredoc "<<") of the redirection data.
	This rule is both examples below give the same result:

			EXAMPLE:
			> hola echo file3.txt
			cat file3.txt
			cat: file3.txt: No such file or directory
			cat hola
			file3.txt

			EXAMPLE:
			> file3.txt echo file3.txt
			cat file3.txt
			file3.txt

			EXAMPLE:
			echo file3.txt > hola
			cat file3.txt
			cat: file3.txt: No such file or directory
			cat hola
			file3.txt

			EXAMPLE:
			echo file3.txt > file3.txt
			cat file3.txt
			file3.txt

		If the redirection symbol is followed by another redirection symbol or a pipe it will be considered an error

			EXAMPLE:
			echo hello > |
			bash:syntax error near unexpected token `|'

			EXAMPLE:
			echo hello > >
			bash: syntax error near unexpected token `>'

			EXAMPLE:
			echo hello > <
			bash: syntax error near unexpected token `<'

			EXAMPLE:
			echo hello >> <
			bash: syntax error near unexpected token `<'

			EXAMPLE:
			echo hello << |
			bash:syntax error near unexpected token `|'

		If the redirection data (symbol + file) is followed by another redirection data (symbol + file) it is not an error.
		We symply have 2 redirections happening in the same node, from my understanding we can have as many redirctions as we want in the same node.
		IMPORTANT: if we have a command, it will be executed in the last redirection of the node.

			EXAMPLE:
			echo hello > file4.txt >file5.txt >file6.txt
			----> file4 and file5 are empty but file6.txt contains the text "hello"

			EXAMPLE
			echo > file4.txt >file5.txt hello >file6.txt
			----> since bash sees that the node is a command, "hello" is not see as redir data (redir + file) and will be memorized as argument of echo
			----> file4 and file5 are empty but file6.txt will still contain the text "hello"

			EXAMPLE
			> file4.txt >file5.txt echo hello >file6.txt
			----> since bash sees that the node is a command, "hello" is not see as redir data (redir + file) and will be memorized as argument of echo
			----> file4 and file5 are empty but file6.txt will still contain the text "hello"

		If the redirection data is followed by a path /dir/dir/file, it will not create a file called
		"/dir/dir/file" but will instead create file in the path you are given it to.
		If the path does not exist bash will give an error

			EXAMPLE - invalid path
			echo hello > dir/dir/file
			bash: dir/dir/file: No such file or directory

			EXAMPLE - valid path
			echo hello > /Users/jjs/file
			cat file
			hello

			EXAMPLE - If the token after > is a directory (not a file), Bash will throw an error:
			 echo hello > /Users/jjs
			 bash: /Users/jjs: Is a directory

		ATTENTION! THIS HAPPENS ALSO WITH ENVIRONMENT VARIABLES LIKE $PATH

			EXAMPLE:
			echo hello > $HOME/file2
			cat file2
			hello

	- RULE_N_4: Input cannot end with a redirection symbol.

			EXAMPLE:
			echo hello >
			bash: syntax error near unexpected token `newline'

			EXAMPLE:
			echo hello <
			bash: syntax error near unexpected token `newline'

			EXAMPLE:
			echo hello <<
			bash: syntax error near unexpected token `newline'

			EXAMPLE:
			echo hello >>
			bash: syntax error near unexpected token `newline'

	- RULE_N_5: Input cannot begin with a pipe.

		EXAMPLE:
		| echo hello
		bash: syntax error near unexpected token `|'


	- RULE_N_6: (WE DO NOT NEED TO DO THIS AND MAKE IT LOOK LIKE AN ERROR)
	If input ends with a pipe, bash prompts the user to write commands on the next line, bash will then tokenize the new input and parse it as
	part of the same tree (but different node as a pipe always starts a new node).

		EXAMPLE:
		echo hello |
		> cat
		hello
		NOTE: it's the exact same if we write "echo hello | cat"

	- RULE N_7: with HEREDOC redirections (<<) we can use any name for delimeter.
	Bash will hend the heredoc creation when we write the delimeter (no spaces before or after, no words before or after the word, just the delimeter).

		EXAMPLE:
		bash-3.2$ >> delimeter
		bash-3.2$ << delimiter
		> hello
		> ciao
		> hola
		> hallo
		> moin
		>	  delimiter
		>delimiter		  <-spaces after delimeter
		> delimiter yo
		> delimiter

	- RULE N_8: if we have more than 1 HEREDOC redir in our node we create a buffer for each redirection.
	The first buffer is part of teh first heredoc buff and the second buffer is part of the 2nd heredoc buff.
	We have to write 1t the delimiter for the 1st HEREDOC to use the delimiter of the 2nd heredoc.

	EXAMPLE:
	echo hello << del1 << del2
	> echo hello
	> del2			 <-- this does not work as we are in 1st heredoc with del1 as delimiter
	> del2
	> del1			 <-- heredoc 1 ends and collects previous lines in its buffer
	> del1			 <-- we are in 2nd heredoc with del2 as delimiter so this does not interrupt the heredoc process
	> del1
	> del2			  <-- heredoc  2 ends and collects previous lines in its buffer
	hello

	bash executes from the first to the last herdoc, the buff of the last heredoc
	overwrites the buff of the previous heredocs and is the one executed as a result

	EXAMPLE (with cat):
	cat << first << second
	This goes to the first heredoc buffer
	first
	First heredoc finished, so this goes to the second heredoc buffer
	second
	First heredoc finished, so this goes to the second heredoc buffer

	EXAMPLE (example with file1 out redirection)
	cat << EOF1 << EOF2 > file1
	> echo hello
	> ciao
	> hola
	> EOF2
	> we are still in EOF1 writing EOF2 does nothig and we are still in buff1
	> EOF1
	> now we are in EOF2
	> what do you thinnk will it write to the file?
	> EOF2
	cat file1
	now we are in EOF2
	what do you thinnk will it write to the file?

	RULE N_9: a command's OPTION is seen as such only if it comes next to the command token

		We only use -n with echo as an OPTION.
		This option suppresses appending the new line to the echoed arguments.

		EXAMPLE:
		 bash-3.2$ echo -n hello world
		hello worldbash-3.2$

		If -n is not used right after echo, -n is just an argument

		EXAMPLE:
		bash-3.2$ echo hello -n world
		hello -n world

		EXAMPLE
		bash-3.2$ echo hello world -n
		hello world -n

		EXAMPLE
		bash-3.2$ echo hello -n world -n
		hello -n world -n

		If we write -n multiple times after echo we have a weird behaviour which
		will repress apprending the new line but will write - as last argument!!!

		EXAMPLE:
		bash-3.2$ echo -n -n hello world
		hello worldbash-3.2$

		EXAMPLE:
		bash-3.2$ echo -n -n -n hello world
		hello worldbash-3.2$

*/
