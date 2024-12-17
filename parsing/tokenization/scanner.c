
#include "../../headers/minishell.h"


t_scanner scanner_value(t_char_itr char_itr)
{
	t_scanner scanner;
	t_token token;

	ft_memset(&token, 0, sizeof(t_token));
	scanner.char_itr = char_itr;
	scanner.next = token;
	return(scanner);
}

int scanner_has_next(const t_scanner *self)
{
	char c;
	c = char_itr_peek(&self->char_itr);
	if (!(c == EOF || c == '\n' || c == '\0'))
	{
		return (!(c == EOF || c == '\n' || c == '\0'));
	}
	else
	{
		end_token((t_scanner *)self);
		return (!(c == EOF || c == '\n' || c == '\0'));
	}
}

t_token scanner_next(t_scanner *self)
{
	skip_whitespaces(&self->char_itr);
	if (scanner_has_next(self))
	{
		self->next.lexeme.length = 0;
		self->next = scanner_peek(self);
	}
	else
	{
		self->next = end_token(self);
	}
	return (self->next);
}
// In this moment peek is also consuming the token, it is not just only peeking.. it also advance the iterator along the string.
// We need tocheck if this can be problematic for the use of the parser.
t_token scanner_peek(t_scanner *self)
{
	char c;
	c = *self->char_itr.cursor;
	while(1)
	{
		if (c == EOF || c == '\n' || c == '\0')
			return (end_token(self));
		else if (c == '|')
			return (pipe_token(self));
		else if (c == '>')
			return (redir_out_token(self));
		else if (c == '<')
			return (redir_in_token(self));
		else if (c == '$')
			return (env_var_token(self));
		/* else if (c == '/')
			return (abs_path_token(self));
		else if (c == '.' && ((*(self->char_itr.cursor + 1) == '/') || (*(self->char_itr.cursor + 1) == '.' && *(self->char_itr.cursor + 2) == '/')))
			return (rel_path_token(self));
		else if (c == '-')
			return (option_token(self)); */
		else if (ft_isalnum(c) || ft_strchr(NOBRKSYMS, *self->char_itr.cursor))
			return (word_token(self));
		else
			return (tmp_unknown_token(self));
	}
}
 int check_start_uknown(const char *input)
 {
	if (ft_strchr(SYMBOLS, input[0]))
	{
		return(1);
	}
	else
		return(0);
 }
