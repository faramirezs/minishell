#include "../../headers/minishell.h"


t_token new_token (t_token_type type, char *start, size_t length)
{
	t_token token;

	token.type = type;
	token.lexeme.length = length;
	token.lexeme.start = start;

	return (token);
}

void print_token(const t_token token)
{
	printf("[Token Type]: %d\n", token.type);
	printf("[Token Lexeme]: %.*s\n", (int)token.lexeme.length, token.lexeme.start);
}

t_token end_token (t_scanner *self)
{
	self->next.type = END;
	self->next.lexeme.length = 1;
	self->next.lexeme.start = self->char_itr.cursor;
	return (self->next);
}

t_token pipe_token (t_scanner *self)
{
	self->next.type = PIPE;
	self->next.lexeme.length = 1;
	self->next.lexeme.start = self->char_itr.cursor;
	self->char_itr.cursor++;
	return (self->next);
}

t_token redir_out_token (t_scanner *self)
{
	if (*(self->char_itr.cursor + 1) == '>')
		self->next = append_out_token(self);
	else
	{
		self->next.type = REDIR_OUT;
		self->next.lexeme.length = 1;
		self->next.lexeme.start = self->char_itr.cursor;
		self->char_itr.cursor++;
	}
	return (self->next);
}

t_token redir_in_token (t_scanner *self)
{
	if (*(self->char_itr.cursor + 1) == '<')
		self->next = heredoc_token(self);
	else
	{
		self->next.type = REDIR_IN;
		self->next.lexeme.length = 1;
		self->next.lexeme.start = self->char_itr.cursor;
		self->char_itr.cursor++;
	}
	return (self->next);
}

t_token word_token (t_scanner *self)
{
	self->next.lexeme.start = self->char_itr.cursor;
	self->next.lexeme.length = 1;
	//while(self->char_itr.cursor && ft_isalnum(*self->char_itr.cursor))
	while(self->char_itr.cursor && (ft_isalnum(*self->char_itr.cursor) || ft_strchr(NOBRKSYMS, *self->char_itr.cursor)))
	{
		/* printf("Char: %c, isalnum: %d, strchr: %p, Token lexeme length: %zu in file %s at line %d\n",
			*self->char_itr.cursor,
			ft_isalnum(*self->char_itr.cursor),
			ft_strchr(NOBRKSYMS, *self->char_itr.cursor),
			self->next.lexeme.length,
			__FILE__,
			__LINE__); */
		self->next.lexeme.length++;
		if(char_itr_has_next(&self->char_itr))
			self->char_itr.cursor++;
		else
			break;
	}
	check_cmd(self);
	return (self->next);
}

t_token tmp_unknown_token (t_scanner *self)
{
	self->next.type = UNKNOWN;
	self->next.lexeme.length = 1;
	self->next.lexeme.start = self->char_itr.cursor;
	self->char_itr.cursor++;
	return (self->next);
}

t_token env_var_token(t_scanner *self)
{
	self->next.type = ENV_VAR;
	self->next.lexeme.start = self->char_itr.cursor++;
	self->next.lexeme.length++;
	while (self->char_itr.cursor && (ft_isalnum(*self->char_itr.cursor) || *self->char_itr.cursor == '_'))
	{
		self->next.lexeme.length++;
		self->char_itr.cursor++;
	}
	return (self->next);
}

t_token abs_path_token(t_scanner *self)
{
	self->next.type = ABS_PATH;
	self->next.lexeme.start = self->char_itr.cursor;
	while (self->char_itr.cursor && *self->char_itr.cursor != ' ' && *self->char_itr.cursor != '\t' && *self->char_itr.cursor != '\n')
	{
		self->next.lexeme.length++;
		if(char_itr_has_next(&self->char_itr) == 1)
			self->char_itr.cursor++;
		else
			break;
	}
	return (self->next);
}

t_token rel_path_token(t_scanner *self)
{
	self->next.type = REL_PATH;
	self->next.lexeme.start = self->char_itr.cursor;
	while (self->char_itr.cursor && *self->char_itr.cursor != ' ' && *self->char_itr.cursor != '\t' && *self->char_itr.cursor != '\n')
	{
		self->next.lexeme.length++;
		if(char_itr_has_next(&self->char_itr) == 1)
			self->char_itr.cursor++;
		else
			break;
	}
	return (self->next);
}

t_token option_token(t_scanner *self)
{
	self->next.type = OPTION;
	self->next.lexeme.start = self->char_itr.cursor++;
	self->next.lexeme.length = 2;
	self->char_itr.cursor++;
	return (self->next);
}

t_token append_out_token(t_scanner *self)
{
	self->next.type = APPEND_OUT;
	self->next.lexeme.length = 2;
	self->next.lexeme.start = self->char_itr.cursor;
	self->char_itr.cursor += 2;
	return (self->next);
}

t_token heredoc_token(t_scanner *self)
{
	self->next.type = HEREDOC;
	self->next.lexeme.length = 2;
	self->next.lexeme.start = self->char_itr.cursor;
	self->char_itr.cursor += 2;
	return (self->next);
}
