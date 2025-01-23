/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   token.h											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: alramire <alramire@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/01/10 17:23:09 by alramire		  #+#	#+#			 */
/*   Updated: 2025/01/10 17:23:55 by alramire		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "minishell.h"

/*
	COMMAND,				// 0 These are the commands we need to create ("echo, cd, pwd, export, unset, env, exit")
	ENV_VAR,				// 1 Environment variable are words that start with $ (e.g., "$HOME", "$USER"), we do error handling for this after the tokenizing and node creation
	ABS_PATH,		   	// 2 A path starting with / (e.g., /directory/directory/  or /directory/file ) is an absolute path and refers to the exact location of the file or command file system staring from root directory
	REL_PATH,		   	// 3 A path that doesn’t start with ./ or ../ It is relative to the current working directory. (example ./directory/directory/  or ../directory/directory/  or ../directory/directory/ or ../directory/file )
	OPTION,		 		// 4 Command options (only "-n" with echo needs to handled), if not "-n" it should be UKNOWN (unless if the -n is bwtween quotes i think)

	REDIR_IN,	   		// 5 Input redirection ("<")
	REDIR_OUT,	  		// 6 Output redirection (">")
	APPEND_OUT,	 		// 7 Append redirection (">>")
	HEREDOC,				// 8 Here-document redirection ("<<")

	PIPE,		   		// 9 Pipe operator ("|")
	STRING_D_QUOTES,		// 10 Strings inside double quotes (e.g., "hello world"), after the string is ready we have to add a function that will see if we have $var inside here because it can be expanded inside double quotes only, we should also have a function that cheks if there is a command token type only inside
	STRING_S_QUOTES,		// 11 String inside single quotes (e.g., "hello world"), after the string is ready we should have a function that cheks if there is a command token type only inside

	WORD,		 			// 12 Any letter or number that is not surrounded in " " and '', after we create the tokens and nodes, we need a function that will see if the word is an external command or just a word
	UNKNOWN,		 		// 13 An invalid token type is a symbol that our shell won't have to execute: "\", ";", "&&", "||", unclosed " and ', "(", ")", "#", "&", "$(...)", `backticks, "*", "~" etc.. --- what about tilde?
	END,					// 14 END token


	// I DON'T THINK WE NEED THESE ESPECIALLY BECAUSE THESE DETAILS ARE PART OF THE INVALID TOKENS CATEGORY OR THINGS WE WILL CHECK AFTER WE PARSE TOKENS TO NODES
	//EXTERNAL_COMMAND	  // commands like grp or ls that we do not have to create, we can access these as binaries in /bin or /usr/bin, we do not need to create tokens for these, after we create our tokens and nodes we will then see if word is a external command
	// D_QUOTE,		 	// we can handle unclosed quotes as an unknown token, we can tokenize closed double quotes strings as STRING_D_QUOTES
	// S_QUOTE,		 	// we can handle unclosed quotes as an unknown token, we can tokenize closed double quotes strings as STRING_D_QUOTES
	// DELIMITER,	  		// we don't hav  to handle ; as a delimiter as it is required by the bonus, the symbol ; be seen as error if not inside a string
	// OPERATOR,	   		// we don't hav  to handle logical operators for `&&` and `||` as they are required by the bonus, these symbols will be seen as an error if not inside a string
	// FILENAME,	   		// i don't think we need this as we will check if something is a file based on the commands we find in our nodes
	// SUBSHELL,	   		// we do not need to handle subshell
	// COMMENT,				// we do not need to handle comments
	// PATH_EXP,			// we do not need this
 */

typedef enum e_token_type // good to have an order like below
{
	COMMAND,
	ENV_VAR,
	ABS_PATH,
	REL_PATH,
	REDIR_IN,
	REDIR_OUT,
	APPEND_OUT,
	HEREDOC,
	PIPE,
	STRING_D_QUOTES,
	STRING_S_QUOTES,
	WORD,
	UNKNOWN,
	END,
}					t_token_type;

typedef struct s_slice
{
	const char		*start;
	size_t			length;
}					t_slice;

typedef struct s_token
{
	t_token_type	type;
	t_slice			lexeme;
}					t_token;

t_token		new_token(t_token_type type, char *start, size_t length);
void		print_token(const t_token token);
int			compare_token(const t_token *token, const char *str);
/*
t_token		single_quote_token(t_scanner *self, t_context *msh);
t_token		double_quote_token(t_scanner *self, t_context *msh);
char 		*get_env_vvalue(t_scanner *self, t_context *msh);
t_slice 	expand_env_var(t_scanner *self, t_context *msh);
*/
#endif
