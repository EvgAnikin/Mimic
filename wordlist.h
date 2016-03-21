typedef struct wordlist wordlist;
struct wordlist
{
	word *wrd;
	wordlist *next;
};
wordlist *newitem(word *w);
wordlist *add_front(wordlist *list, word *element);
wordlist *pick_random(wordlist *list);
void *clear_wordlist(wordlist *list);
