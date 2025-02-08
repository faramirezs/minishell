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

// t_token word_token (t_scanner *self)
// {
// 	self->next.lexeme.start = self->char_itr.cursor;
// 	//self->next.lexeme.length = 1;
// 	//while(self->char_itr.cursor && ft_isalnum(*self->char_itr.cursor))
// 	while(self->char_itr.cursor && *self->char_itr.cursor != '\0' && (ft_isalnum(*self->char_itr.cursor) || ft_strchr(NOBRKSYMS, *self->char_itr.cursor)))
// 	{
// 		/* printf("Char: %c, isalnum: %d, strchr: %p, Token lexeme length: %zu in file %s at line %d\n",
// 			*self->char_itr.cursor,
// 			ft_isalnum(*self->char_itr.cursor),
// 			ft_strchr(NOBRKSYMS, *self->char_itr.cursor),
// 			self->next.lexeme.length,
// 			__FILE__,
// 			__LINE__); */
// 		self->next.lexeme.length++;
// 		if(char_itr_has_next(&self->char_itr))
// 			self->char_itr.cursor++;
// 		else
// 			break;
// 	}
// 	check_cmd(self);
// 	return (self->next);
// }

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

t_token non_delimited_token(t_scanner *self)
{
    char *temp;

    temp = ft_strdup("");
    while (*self->char_itr.cursor)
    {
        if (*self->char_itr.cursor == '"')
            temp = ft_strjoin_free_s1(temp, double_quote_token(self).lexeme.start);
        else if (*self->char_itr.cursor == '\'')
            temp = ft_strjoin_free_s1(temp, single_quote_token(self).lexeme.start);
        
        else
        {
            temp = ft_strjoin_free_s1(temp, ft_substr(self->char_itr.cursor, 0, 1));
            self->char_itr.cursor++;  // ✅ Move forward normally
        }

        if (*self->char_itr.cursor == '\0' || ft_strchr(" \t\n|><", *self->char_itr.cursor))
            break ;  // ✅ Stop at delimiters like space, pipes, or redirections
    }

    self->next.type = WORD;
    self->next.lexeme.start = temp;
    self->next.lexeme.length = ft_strlen(temp);
    return self->next;
}



t_token double_quote_token(t_scanner *self)
{
    char    *expanded;
    t_token var;
    char    *continuation;


    self->next.type = STRING_D_QUOTES;
    self->next.lexeme.start = ++self->char_itr.cursor;
    expanded = ft_strdup("");
    while (1)
    {
        if (*self->char_itr.cursor == '"')
        {
            if(char_itr_has_next(&self->char_itr))
			{
				self->char_itr.cursor++;
				if(*self->char_itr.cursor == '"' && char_itr_has_next(&self->char_itr))
					self->char_itr.cursor++;
				else
					break ;
			}
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
        if (*self->char_itr.cursor == '$' && *(self->char_itr.cursor + 1)
            && find_env_index(self->msh->env, self->char_itr.cursor + 1))
        {
            var = handle_expansions (self);
            expanded = ft_strjoin_free_s1(expanded, var.lexeme.start);
            //self->char_itr.cursor += var.lexeme.length;  // Move past the expanded variable
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
    char        *continuation;
    char        *expanded;
    const char  *start;

    self->next.type = STRING_S_QUOTES;
    start = ++self->char_itr.cursor; // Store start position (skip opening quote)
    expanded = ft_strdup(""); // Start with an empty string

    while (1)
    {
        if (*self->char_itr.cursor == '\'')  // Found closing quote
        {
            self->char_itr.cursor++;
            break ;
        }
        if (*self->char_itr.cursor == '\0')  // Handle multi-line input
        {
            continuation = readline("quote> ");
            if (!continuation)  // User pressed Ctrl+D
            {
                fprintf(stderr, "minishell: unexpected EOF while looking for matching `'\n");
                self->msh->ret_exit = 2;
                self->next.type = UNKNOWN;
                free(expanded);
                return self->next;
            }
            expanded = ft_strjoin_free_s1(expanded, continuation);
            free(continuation);
            continue;
        }
        expanded = ft_strjoin_free_s1(expanded, ft_substr(self->char_itr.cursor, 0, 1));
        self->char_itr.cursor++; // ✅ Move forward normally
    }
    
    self->next.lexeme.start = expanded;  // ✅ Store the extracted string
    self->next.lexeme.length = ft_strlen(expanded);

    return (self->next);
}

char *expand_special_vars(char c, t_context *msh)
{
    if (c == '$')  // ✅ Handle `$$` (Process ID)
        return ft_itoa(getpid());
    if (c == '?')  // ✅ Handle `$?` (Last exit code)
        return ft_itoa(msh->ret_exit);
    return NULL;
}

char *expand_normal_var(t_scanner *self)
{
    char var_name[256];
    int i = 0;
    char *value;

    // ✅ Extract variable name (letters, digits, underscores)
    while (ft_isalnum(*self->char_itr.cursor) || *self->char_itr.cursor == '_')
    {
        if (i < 255)
            var_name[i++] = *self->char_itr.cursor;
        self->char_itr.cursor++;
    }
    var_name[i] = '\0';  // Null-terminate

    // ✅ Get environment variable value
    value = ms_get_env(self->msh->env, var_name);
    if (!value)
        return ft_strdup("");  // Variable not found → empty string
    return ft_strdup(value);
}

t_token handle_expansions(t_scanner *self)
{
    t_token token;
    char *value;
    char *suffix;
    char *temp;

    self->char_itr.cursor++;
    if (!ft_isalnum(*self->char_itr.cursor) && *self->char_itr.cursor != '?' && *self->char_itr.cursor != '_')
    {
        token.type = WORD;
        token.lexeme.start = ft_strdup("$");
        token.lexeme.length = 1;
        return token;
    }
    value = expand_special_vars(*self->char_itr.cursor, self->msh);
    if (value)
        self->char_itr.cursor++;
    else
        value = expand_normal_var(self);
    if (*self->char_itr.cursor && (ft_isalnum(*self->char_itr.cursor) || *self->char_itr.cursor == '_'))
    {
        suffix = ft_substr(self->char_itr.cursor, 0, ft_strlen(self->char_itr.cursor));
        temp = ft_strjoin(value, suffix);
        free(value);
        free(suffix);
        value = temp;
        self->char_itr.cursor += ft_strlen(self->char_itr.cursor);
    }
    token.type = WORD;
    token.lexeme.start = value;
    token.lexeme.length = ft_strlen(value);
    return token;
}
