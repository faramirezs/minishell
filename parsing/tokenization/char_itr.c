
#include "../../headers/minishell.h"

// value lifetime dependes of the start reference
t_char_itr char_itr_value(const char *star, size_t length)
{
	t_char_itr itr;
	itr.cursor = star;
	itr.sentinel1 = star + length;
	return(itr);
}
//skip_ function advances the cursor to the first non whitespace occurrence.
void skip_whitespaces (t_char_itr *self)
{
	char whitespaces[] = WHITESPACES; //token delimiters
	//const char *start = self->cursor;
	//int count = 0;
	while(self->cursor && self->cursor < self->sentinel1 && ft_strchr(whitespaces, *self->cursor))
	{
			//printf("Space found at %p\n", self->cursor);
			//if (char_itr_has_next(self) == 1)
			char_itr_next(self);
			 // We need to validate for END, because the cursor is advancing even after the last space from the input string.
			//count++;
	}
	//if (count > 0)
	//printf("Skipped %d whitespaces from %p to %p\n", count, start, self->cursor);
	}

int is_whitespace (t_char_itr *self)
{
	char whitespaces[] = WHITESPACES; //token delimiters
	return(self->cursor && self->cursor < self->sentinel1 && ft_strchr(whitespaces, *self->cursor));
}

//Returns true when there are aditional chars to consume on the iterable range
int char_itr_has_next(const t_char_itr *self)
{
	//I have some doubts about this function... Not sure if instead of self->cursor i need to go fo self->cursor + 1. To see what is the next one... But when doing so Am i advancing the iterator? Should i create a copy?
	//printf("Itr has more chars? R: %i\n", self->cursor < self->sentinel1);
	return(self->cursor < self->sentinel1);
}
//peek and return the next character. Do not advance the cursor. Will crash out of bound if no more chars to consume
char char_itr_peek(const t_char_itr *self)
{
	if(char_itr_has_next(self))
	{
		//printf("Cursor peek: %p | char %c\n", self->cursor, *self->cursor);
		return(*self->cursor);
	}
	else if(self->cursor == self->sentinel1)
	{
		//printf("cursor and sentinel are equals\n");
		return(*self->cursor);
	}
	else
	{
		//printf("Cursor %p, Sentinel %p", self->cursor, self->sentinel1);
		fprintf(stderr, "%s:%d - Out of Bounds\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}

}

//read next char and advance cursor. Will crash out of bound if no more chars to consume
char char_itr_next(t_char_itr *self)
{
	if(char_itr_has_next(self))
	{
		//printf("Cursor current: %p | char %c\n", self->cursor, *self->cursor);
		return(*self->cursor++);
	}
	else
	{
		fprintf(stderr, "%s:%d - Out of Bounds\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
}

//Returns pointer to the current location of iterator cursor porinter.
const char* char_itr_cursor(const t_char_itr *self)
{
	//printf("Cursor current: %p | char %c\n", self->cursor, *self->cursor);
	return(self->cursor);
}
