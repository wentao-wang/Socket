#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "list.h"

int Append(List * list, char* word, char* define)
{
    // Create the new node
    ListElem * elem;
    elem = (ListElem *)malloc(sizeof(ListElem));
    if(!elem) // Allocate memory failed
        return 0;
    elem -> word = word;
    elem -> define = define;
    // Insert the node
    ListElem * last_elem = Last(list);
    if(last_elem)
    {
        elem -> next = &(list -> anchor);
        elem -> prev = last_elem;
        (list -> anchor).prev = elem;
        last_elem -> next = elem;
        list -> num_members++;
    }
    else // List is empty
    {
        elem -> next = &(list -> anchor);
        elem -> prev = &(list -> anchor);
        (list -> anchor).next = elem;
        (list -> anchor).prev = elem;
        list -> num_members++;
    }
    return 1;
}


ListElem * Find(List * list, char * word)
{
    ListElem * elem = First(list);
   // printf("%s\n", word );
    while(elem)
    {
    	//printf("%s\n", elem -> word );
        if(strcmp(elem -> word, word) == 0) 
            return elem;
        elem = Next(list, elem);
    }
    return NULL;
}

ListElem * Next(List * list, ListElem * cur)
{
    if(cur == Last(list))
        return NULL;
    else
        return cur -> next;
}

int Init(List * list)
{
    (list -> anchor).next = &(list -> anchor);
    (list -> anchor).prev = &(list -> anchor);
    (list -> anchor).word = NULL;
    (list -> anchor).define = NULL;
    list -> num_members = 0;
    return 1;
}


ListElem * Last(List * list)
{
    if(list -> num_members <= 0)
        return NULL;
    else
        return (list -> anchor).prev;
}

ListElem * First(List * list)
{
    if(list -> num_members <= 0)
        return NULL;
    else
        return (list -> anchor).next;
}

// int main() {
// 	List* list1;
// 	memset(list1, 0, sizeof(List));
// 	Init (list1);
// 	char* w = "haha";
// 	char* d = "h def";
// 	Append(list1, w, d) ;
// 	printf("%s\n", Find(list1, "haha")->define );
// }