enum
{
	NHASH = 4093,
	MULTIPLIER = 31,
};

typedef struct word word;
struct word
{
	char *wr;
	word *next;
};

word *wordtab[NHASH];

unsigned int hash(char *wr);
void init_wordtab();
word *wlookup(char *wr, int create);
void clear_wordtab(void);
