#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "wordtab.h"
#include "wordlist.h"

enum
{
	NPREF = 2,
	MAXGEN = 200,
	MAXLEN = 100
};

typedef struct state state;

struct state
{
	word *pref[NPREF];
	wordlist *suflist;
	state *next;
};

state *statetab[NHASH];

unsigned int prefhash(word *pref[NPREF])
{
	unsigned int h = 0;
	int i;
	char *letter;
	for(i = 0; i < NPREF; i++)
		for(letter = pref[i]->wr; *letter != '\0'; letter++)
		{
			h = MULTIPLIER*h + *letter;
		}
	return h % NHASH;
}

void init_statetab()
{
	int i;
	for(i = 0; i < NHASH; i++)
	{
		statetab[i] = NULL;
	}
}

int pref_equal(word *pref1[NPREF], word *pref2[NPREF])
{
	int i;
	for(i = 0; i < NPREF; i++)
	{
		if(pref1[i] != pref2[i]) return 0;
	}
	return 1;
}

state *slookup(word *pref[NPREF], int create)
{
	unsigned int index = prefhash(pref);
	int i;
	state *item;
	for(item = statetab[index]; item != NULL; item = item->next)
	{
		if(pref_equal(item->pref, pref))
		{
			return item;
		}
	}
	if(create)
	{
		state *new;
		new = (state *)malloc(sizeof(state));
		new->suflist = NULL;
		new->next = statetab[index];
		for(i = 0; i < NPREF; i++)
		{
			new->pref[i] = pref[i];
		}
		statetab[index] = new;
		return new;
	}
	else return NULL;
}

void add(word *pref[NPREF], word *suf)
{
	state *st;
	st = slookup(pref, 1);
	st->suflist = add_front(st->suflist, suf);
}

void build(FILE *stream, word *firstpref[NPREF])
{
	char buf[MAXLEN];
	char fmt[10];
	word *pref[NPREF];
	word *suf;
	int i;

	sprintf(fmt, "%%%ds", MAXLEN-1);
	for(i = 0; i < NPREF; i++)
	{
		if(fscanf(stream, fmt, buf) == EOF)
		{
			printf("Input file too short");
			exit(2);
		}
		firstpref[i] = pref[i] = wlookup(buf, 1);
	}
	while(fscanf(stream, fmt, buf) != EOF)
	{
		suf = wlookup(buf, 1);
		add(pref, suf);
		memmove(pref, (pref+1), (NPREF-1)*sizeof(pref[0]));
		pref[NPREF-1] = suf;
	}
}

void clear_statetab(void)
{
	int i;
	state *item, *next;
	for(i = 0; i < NHASH; i++)
	{
		for(item = statetab[i]; item != NULL; item = next)
		{
			next = item->next;
			clear_wordlist(item->suflist);
			free(item);
		}
	}
}

void generate(int n, word *firstpref[NPREF])
{
	int i;
	word *pref[NPREF];
	word *suf;
	state *st;

	for(i = 0; i < NPREF; i++)
	{
		pref[i] = firstpref[i];
		printf("%s ", pref[i]->wr);
	}

	for(i = 0; i < n; i++)
	{
		st = slookup(pref, 0);
		if(st == NULL || st->suflist == NULL) break;

		suf = pick_random(st->suflist)->wrd;
		printf("%s ", suf->wr);
		memmove(pref, (pref+1), (NPREF-1)*sizeof(pref[0]));
		pref[NPREF-1] = suf;
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	word *firstpref[NPREF];

	srand(time(NULL));
	init_wordtab();
	init_statetab();
	build(stdin, firstpref);
//	printf("Built\n");
	generate(MAXGEN, firstpref);
	clear_wordtab();
	clear_statetab();
}
