/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   how_bash_parses_tokens.c						   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: jslusark <jslusark@student.42berlin.de>	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/11/17 15:43:09 by jslusark		  #+#	#+#			 */
/*   Updated: 2024/11/17 17:48:42 by jslusark		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */



/*
1. Bash interprets the input of the user based on the order in which we write it and while considering these 3 main data types (commands, redirections, pipes).
2. Bash will always interpret the first word in the input as a command of a node, unless that word is a redirection symbol or a pipe.
3. When Bash finds a redirection symbol (>, >, >> and <<) it expects that the word after the redirection symbol is a file.
4. If the redirection symbol (>, >, >> and <<) is followed by another redirection symbol (>, >, >> and <<) or pipe it is an ERROR.
4. A node ends when the word of the input is a pipe or a new line.
5. When Bash finds a pipe it will expect to start a new node with the words that come after the pipe.
6. If an input ends with pipe and no other words come after pipe, bash will prompt the user to enter other input thatwill be tokenized and parsed as part of the tree.


HERE ARE SOME EXAMPLES WHERE I EXPLAIN STEP BY STEP HOW BASH BEHAVES BASED ON THE INPUT WE WRITE TO IT.
FEEL FREE TO TEST THESE CASES AND COME OUT WITH OTHERS WHERE YOU CAN SEE IF IT FOLLOWS THE SAME LOGIC;.


1. EXAMPLES WITH 1 NODE.

		- EXAMPLE WITH COMMAND + OUTPUT REDIRECTION:

			EXAMPLE 1:
				echo hello > file
				cat file
				hello

				Bash goes through the input like this:
					- echo  : this is not a redirection or a pipe, since it's the first word i find it must be the command of our first node.
					- hello : i already assigned the command of our node, since this is not a redirection or a pipe it must be an argument of the command "echo".
					- >	 : this is a redirection symbol, so I have to start my redirection data, this is an output redirection and the word that comes after must be a file
					- file  : i saw a redirection symbol before, so this must be the file we have to redirect to.
					-	   : there is nothing else so the node is over, and our tree is made of just one node.


			EXAMPLE 2:
				> file echo hello
				cat file
				hello

				Order of the input is different but gives the same result!
				This happens because Bash goes through the input like this:
					- >	 : this is a redirection symbol, so I have to start my redirection data, this is an output redirection and the word that comes after must be a file
					- file  : i saw a redirection symbol before, so this must be the file we have to redirect to.
					- echo  : since this is the first word i find that is not redirection or pipe, it must be the command of our node.
					- hello : i already assigned the command of our node, since this is not a redirection or a pipe it must be an argument of the command "echo".
					-	   : there is nothing else so the node is over, and our tree is made of just one node.

			EXAMPLE 3:
				echo > echo hello
				cat echo
				hello

				But we wrote  echo after the redirection.. how is this possible?!!
				This happens because Bash goes through the input like this:
					- echo  : since this is the first word i find that is not redirection or pipe, it must be the command of our node.
					- >	 : this is a redirection symbol, so I have to start my redirection data, this is an output redirection and the word that comes after must be a file
					- echo  :  i saw a redirection symbol before, so this must be the file we have to redirect to. (bash doesn't care how this file is called)
					- hello : i already assigned the command of our node, since this is not a redirection or a pipe it must be an argument of the command "echo".
					-	   : there is nothing else so the node is over, and our tree is made of just one node.

			EXAMPLE 4:
				> file hello echo
				hello: command not found

				Why does it see "hello" as a command?! I just wrote echo after!
				This happens because Bash goes through the input like this:
					- >	  : this is a redirection symbol, so I have to start my redirection data, this is an output redirection and the word that comes after must be a file
					- file   : i saw a redirection symbol before, so this must be the file we have to redirect to.(bash doesn't care how this file is called)
					- hello  : since this is the first word I find that is not redirection or pipe, it must be the command of our node.
					- echo   : i already assigned the command of our node, since this is not a redirection or a pipe it must be an argument of the command "hello".
					-		: there is nothing else to process for this node. Our tree is made of one node.
					Bash then tries to execute the command of the node and sees that "hello" is not an actual command.

			EXAMPLE 5:
				> echo hello
				hello: command not found

				Why does it see "hello" as a command?! I just wrote echo before hello!
				This happens because Bash goes through the input like this:
					- >	  : this is a REDIRECTION symbol, so I have to start my redirection data, this is an output redirection and the word that comes after must be a file
					- echo   : i saw a redirection symbol before, so this must be the file we have to redirect to. (bash doesn't care how this file is called)
					- hello  : since this is the first word I find that is not redirection or pipe, it must be the command of our node.
					-		: there is nothing else so the node is over, and our tree is made of just one node.
				Bash then tries to execute the command of the node and sees that "hello" is not an actual command.


2. EXAMPLES THAT HAVE MORE THAN 1 NODES

			EXAMPLE 1:
				echo hello > file1 | echo ciao > file2
				cat file1
				hello
				cat file2
				ciao

			Bash goes through the input like this:

					NODE1:
					- echo  : since this is the first word i find that is not redirection or pipe, it must be the command of our 1st node.
					- hello : i already assigned the command of our node, since this is not a redirection or a pipe it must be an argument of the command "echo".
					- >	 : this is a REDIRECTION symbol, so I have to start my redirection data, this is an output redirection and the word that comes after must be a file
					- file1  : we had a redircetion before so this must be the file we have to redirect to.
					- |	 : we found a pipe so we end our node and start a new one.

					NODE 2:
					- echo  : since this is the first word i find that is not redirection or pipe, it must be the command of our 2nd node.
					- ciao : i already assigned the command of our node, since this is not a redirection or a pipe it must be an argument of the command "echo".
					- >	 : this is a REDIRECTION symbol, so I have to start my redirection data, this is an output redirection and the word that comes after must be a file
					- file2  : we had a redircetion before so this must be the file we have to redirect to.
					-		: there is nothing else so node 2 is over. When we have more than 1 nodes we have built a node table (sequence of nodes). Out tree is made of 1 node table of 2 nodes.

					IMPORTANT: pipe is what connects one node to the other inside a node table


				EXAMPLE 2:
				echo hello > file |
				>cat
				hello

				will check on this better later but already have an idea how it will work based on my implememtation

 */
