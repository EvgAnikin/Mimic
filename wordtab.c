#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordtab.h"

unsigned int hash(char *wr)
{
	unsigned int h = 0;
	int i;
	char *letter;
	for(letter = wr; *letter != '\0'; letter++)
	{
		h = MULTIPLIER*h + *letter;
	}
	return h % NHASH;
}


void init_wordtab()
{
	int i;
	for(i = 0; i < NHASH; i++)
	{
		wordtab[i] = NULL;
	}
}

word *wlookup(char *wr, int create)
{
	unsigned int index = hash(wr);
	word *item;
	for(item = wordtab[index]; item != NULL; item = item->next)
	{
		if(strcmp(item->wr, wr) == 0)
		{
			return item;
		}
	}
	if(create)
	{
		word *new;
		new = (word *)malloc(sizeof(word));
		new->next = wordtab[index];
		new->wr = strdup(wr);
		wordtab[index] = new;
		return new;
	}
	else 
		return NULL;	
}

void clear_wordtab(void)
{
	int i;
	word *item, *next;
	for(i = 0; i < NHASH; i++)
	{
		for(item = wordtab[i]; item != NULL; item = next)
		{
			next = item->next;
			free(item->wr);
			free(item);
		}
	}
}
