/*

Found out that bash will still create tokens for | and redirections even when not
separated with a space from the preceding or following word.
Unsure if this applies also to ther token types.
Try the examples below on bash to better understand and see what I mean:

PIPE:

	EXAMPLE 1:
		These commands will tokenize and parse the same way:
		"echo hello | cat"
		"echo hello| cat"
		"echo hello |cat"

		We will receive the following tokens for all the 3 examples above:
		Token 1: echo   (command)
		Token 2: hello  (word)
		Token 3: |	  (pipe)
		Token 4: cat	(command)

		"echo hello| cat" will not tokenize this way - THIS IS WRONG:
		Token 1: echo   (command)
		Token 2: hello| (word/unknown
		Token 3: cat	(command)


		"echo hello |cat" will not tokenize this way - THIS IS WRONG:
		Token 1: echo   (command)
		Token 2: hello (word/unknown)
		Token 3: |cat	(command)

	EXAMPLE 2:
		These commands will tokenize and parse the same way
		"echo hello |"
		"echo hello|"

		We will receive the following tokens for all the 3 examples above:
		Token 1: echo   (command)
		Token 2: hello  (word)
		Token 3: |	  (pipe)

		And it will not tokenize this way - THIS IS WRONG:
		"echo hello|"
		Token 1: echo   (command)
		Token 2: hello| (word/unknown)


REDIRECTIONS:

	- EXAMPLE WITH REDIR OUTPUT:
		These commands will all act the same and write "this is still a redir" to the redirected file:
		"echo this is still a redir › sillyfile.txt"
		"echo this is still a redir ›sillyfile.txt"
		"echo this is still a redir>sillyfile.txt"

		We will receive the following tokens for all the 3 examples above:
		Token 1: echo   (command)
		Token 2: this  (word)
		Token 3: is  (word)
		Token 4: still  (word)
		Token 5: a  (word)
		Token 6: redir  (word)
		Token 7: > (redirection output)
		Token 8: sillyfile.txt (word)

		"echo this is still a redir ›sillyfile.txt" will not tokenise this way - THIS IS WRONG:
		Token 7: >sillyfile.txt (word)

		"echo this is still a redir›sillyfile.txt" will not tokenise this way - THIS IS WRONG:
		Token 6: redir>sillyfile.txt (word)

PATHS:
	For now the tokenizer should only see that when a word in the input starts with / it is an absolute path
	when word starts with ./ or ../ it is a relative path.
	After creating nodes we can create a function that checks the folders and files of the path and error handles before executing the node it is part of

	ABSOLUTE PATH:
	this will always be the structure, relative path starts with /
	/directory/directory/directory/file
	/directory/directory/directory/
	/directory/ <- when we end a path with / it means the last element is a directory
	/directory/file <- when we end a path without it means the last element is a file

	RELATIVE PATH:
	this will always be the structure, relative path starts with ./ or ../
	./directory/directory/file
	../directory/directory/file
	./directory/directory/
	../directory/directory/

	./ <- means current directory
	../ <- means parent directory
	./directory/ <- when we end a path with / it means the last element is a directory
	./directory/file <- when we end a path without it means the last element is a file
*/
