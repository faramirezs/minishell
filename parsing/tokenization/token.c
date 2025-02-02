#include "../../headers/minishell.h"
//nclude "../../headers/env_var.h"


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
	//self->next.lexeme.length = 1;
	//while(self->char_itr.cursor && ft_isalnum(*self->char_itr.cursor))
	while(self->char_itr.cursor && *self->char_itr.cursor != '\0' && (ft_isalnum(*self->char_itr.cursor) || ft_strchr(NOBRKSYMS, *self->char_itr.cursor)))
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
	t_token token;
	token = scanner_peek(self);
	self->next = token;
	return (token);
	self->next.lexeme.length = 1;
	self->next.lexeme.start = self->char_itr.cursor;
	self->char_itr.cursor++;
	return (self->next);
}

t_token env_var_token(t_scanner *self)
{
    t_slice expanded;

    self->next.type = ENV_VAR;

    // Get the expanded value
    expanded = expand_env_var(self);

    // Store the expansion directly in the scanner's next token
    self->next.lexeme = expanded;

    return self->next;
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
/*
t_token option_token(t_scanner *self)
{
	self->next.type = OPTION;
	self->next.lexeme.start = self->char_itr.cursor++;
	self->next.lexeme.length = 2;
	self->char_itr.cursor++;
	return (self->next);
}
 */
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

t_token double_quote_token(t_scanner *self)
{
	char	*expanded;
	t_slice	var;
	char	*continuation;

    self->next.type = STRING_D_QUOTES;
    self->next.lexeme.start = ++self->char_itr.cursor;
    expanded = ft_strdup("");
    while (1)
    {
        if (*self->char_itr.cursor == '"')
        {
            self->char_itr.cursor++; // Skip closing quote
            break;
        }
        if (*self->char_itr.cursor == '\0')
        {
            continuation = readline("dquote> ");
            if (!continuation)
            {
                fprintf(stderr, "minishell: unexpected EOF while looking for matching `\"'\n");
                self->msh->ret_exit = 2;
                self->next.type = UNKNOWN;
                free(expanded);
                return self->next;
            }
            self->char_itr.cursor = ft_strjoin_free_s1((char *)self->char_itr.cursor, continuation);
            free(continuation);
            continue;
        }
        if (*self->char_itr.cursor == '$')
        {
            var = expand_env_var(self);
            expanded = ft_strjoin_free_s1(expanded, var.start);
        }
        else
        {
            expanded = ft_strjoin_free_s1(expanded, ft_substr(self->char_itr.cursor, 0, 1));
            self->char_itr.cursor++;
        }
    }
    self->next.lexeme.start = expanded;
    self->next.lexeme.length = ft_strlen(expanded);
    return self->next;
}


t_token single_quote_token(t_scanner *self)
{
	char *continuation;

    self->next.type = STRING_S_QUOTES;
    self->next.lexeme.start = ++self->char_itr.cursor; // Skip the opening single quote
    while (1)
    {
        if (*self->char_itr.cursor == '\'')
        {
            self->char_itr.cursor++; // Skip closing quote
            break;
        }
        if (*self->char_itr.cursor == '\0') // Unmatched quote, wait for continuation
        {
            continuation = readline("quote> ");
            if (!continuation)
            {
                fprintf(stderr, "minishell: unexpected EOF while looking for matching `'\n");
                self->msh->ret_exit = 2;
                self->next.type = UNKNOWN;
                return self->next;
            }
            self->char_itr.cursor = ft_strjoin_free_s1((char *)self->char_itr.cursor, continuation);
            free(continuation);
            continue;
        }
        self->next.lexeme.length++;
        self->char_itr.cursor++;
    }
    self->next.lexeme.length = self->char_itr.cursor - self->next.lexeme.start - 1;
    return self->next;
}

char *get_env_vvalue(t_scanner *self)
{
    const char	*start;
    size_t		length;
    char		*var_name;
    char		*value;

    start = self->char_itr.cursor;
    length = 0;

    // Extract the variable name
    while (*self->char_itr.cursor && (ft_isalnum(*self->char_itr.cursor) || *self->char_itr.cursor == '_'))
    {
        length++;
        self->char_itr.cursor++;
    }

    // Get the variable name as a string
    var_name = ft_substr(start, 0, length);

    // Fetch the value from the environment
    value = ms_get_env(self->msh->env, var_name);

    // Clean up
    free(var_name);

    return value;
}

t_slice expand_env_var(t_scanner *self)
{
    char *value;
    t_slice res;

    self->char_itr.cursor++;
    if (*self->char_itr.cursor == '$')
    {
        value = ft_itoa(getpid());
        self->char_itr.cursor++;
    }
    else if (*self->char_itr.cursor == '?')
    {
        value = ft_itoa(self->msh->ret_exit);
        self->char_itr.cursor++;
    }
    else
    {
        value = get_env_vvalue(self);
    }

    if (value)
    {
        res.start = value;
        res.length = ft_strlen(value);
    }
    else
    {
        res.start = NULL;
        res.length = 0;
    }

    return res;
}

// ... existing code ...

char *handle_expansions(const char *arg, t_context *msh)
{
    if (!arg || !*arg)
        return ft_strdup("");

    if (arg[0] == '$')
    {
        char *var_name = ft_strdup(arg + 1);
        char *value = expand_env_var_value(var_name, msh);
        free(var_name);
        return value;
    }

    return ft_strdup(arg);
}
