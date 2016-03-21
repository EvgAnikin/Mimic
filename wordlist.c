#include <stdlib.h>
#include "wordtab.h"
#include "wordlist.h"

wordlist *newitem(word *w)
{
	wordlist *item;
	item = (wordlist *)malloc(sizeof(wordlist));
	item->wrd = w;
	item->next = NULL;
}

wordlist *add_front(wordlist *list, word *w)
{
	wordlist *element;
	element = newitem(w);
	element->next = list;
	return element;
}

wordlist *pick_random(wordlist *list)
{
	int nmatch = 0;
	wordlist *item, *w;
	for(item = list; item != NULL; item = item->next)
	{
		if(rand() % ++nmatch == 0) 
			w = item;
	}
	return w;
}

void *clear_wordlist(wordlist *list)
{
	wordlist *item, *next;
	for(item = list; item != NULL; item = next)
	{
		next = item->next;
		free(item);
	}
}
