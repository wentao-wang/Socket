#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>

#ifndef _LIST_H_
#define _LIST_H_
typedef struct myListElem {
    char *word;
    char *define;
    struct myListElem *next;
    struct myListElem *prev;
} ListElem;

typedef struct myList {
    int num_members;
    ListElem anchor;
    int  (*Append)(struct myList *, char* word, char* define);

    ListElem *(*First)(struct myList *);
	ListElem *(*Last)(struct myList *);
    ListElem *(*Next)(struct myList *, ListElem *cur); 
    ListElem *(*Find)(struct myList *, char* word);
} List;

extern ListElem *First(List*);
extern ListElem *Last(List*);
extern int Append(List*, char*, char* );
extern ListElem *Next(List*, ListElem*);
extern ListElem *Find(List*, char* word);

extern int Init(List*);
#endif /*_LIST_H_*/