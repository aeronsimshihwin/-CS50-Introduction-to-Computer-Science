/**
 * Implements a dictionary's functionality.
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

//by 'hash'ing the word, it will direct the word to the bucket in hashtable where it belongs
int hash(const char* word);

//LENGTH has been defined in dictionary.h and must + 1 'cos 'end of string marker' is considered too
char word[LENGTH + 1];

//count number of words loaded
int wordcount=0;

//define structure node which will be used to form an array of nodes in hashtable
typedef struct node
{
    char *word;
    struct node *next;
}
node;

//value in [] of hashtable[] is the number of buckets in hashtable
node *hashtable[114];

//hash function
//LOL hy if you see this, the time taken is so much longer now. I submitted the 26 buckets one, moving to week 6 first
int hash(const char* word)
{
    int n=0;
    n=strlen(word);
    int bucket=0;
    int bucket1=0;
    int bucket2=0;
    //if-else function for isupper/tolower 'cos bool check is case-insensitive
    if(n==1)
    {
        if(isupper(word[0]))
        {
            bucket=word[0]-'A';
            return bucket;
        }
        else
        {
            bucket=word[0]-'a';
            return bucket;
        }
    }
    else
    {
        bucket=bucket1+bucket2;
        if(isupper(word[0]))
        {
            bucket1=word[0]-'A';
            if(isupper(word[n-1]))
            {
                bucket2=word[n-1]-'A';
            }
            else
            {
                bucket2=word[n-1]-'a';
            }
            return bucket;
        }
        else
        {
            bucket1=word[0]-'a';
            if(isupper(word[n-1]))
            {
                bucket2=word[n-1]-'A';
            }
            else
            {
                bucket2=word[n-1]-'a';
            }
            return bucket;
        }
    }
}

/**
 * Returns true if word is in dictionary else false. Check function must be case-insensitive and assumes strings w only alphabetical characters and/or apostrophes
 */
bool check(const char *word)
{
    //hash(word) will tell us which bucket in hashtable word belongs to
    int bucket=hash(word);

    //allocate memory for pointer
    node *pointer=malloc(sizeof(node));

    //pointer will be pointing at node in bucket of hashtable
    pointer=hashtable[bucket];

    //ensure allocation of memory to pointer was successful
    if(pointer==NULL)
    {
        free(pointer);
        return false;
    }

    //pointer will point at node in hashtable and as long as it is not NULL, it will do strcasecmp
    while(pointer!=NULL)
    {
        //strcasecmp() function puts aside upper and lower cases while comparing
        if(strcasecmp(word, pointer->word)==0)
        {
            return true;
        }
        else
        {
            //pointer will advance as long as the comparison made was different
            pointer=pointer->next;
        }
    }
    free(pointer);
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    //open dictionary, if NULL, tell user
    FILE* dict=fopen(dictionary, "r");
    if(dict==NULL)
    {
        fclose(dict);
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }

    //populate hashtable with nodes containing words found in dictionary
    //scan dictionary word by word and insert a newline after each word
    while(fscanf(dict, "%s\n", word)!=EOF)
    {
        //malloc a cursor for each new word
        node *cursor=malloc(sizeof(node));

        //allocate memory for the word to go into cursor node and + 1 'cos of null terminator
        cursor->word=malloc(sizeof(word) + 1);

        //ensure allocation of memory to cursor was successful
        if(cursor==NULL)
        {
            free(cursor);
            return false;
        }
        else
        {
            //copy word into cursor so cursor->word will have the word from the dictionary
            strcpy(cursor->word, word);

            //hash(word) will tell us which bucket in hashtable word belongs to
            int bucket=hash(word);

            //recall hash function shorts video - hashtable[hash] may end up at the start, middle or end of hashtable
            //start of hashtable will be implemented differently from middle and end of hashtable
            //if at start of hashtable that is empty
            if(hashtable[bucket]==NULL)
            {
                hashtable[bucket]=cursor;
                cursor->next=NULL;
            }
            //else at middle or end of hashtable that is filled
            else
            {
                //next of cursor point to first node of that bucket in hashtable
                cursor->next=hashtable[bucket];
                //point the start of that bucket to the cursor and new linked list is out
                hashtable[bucket]=cursor;
            }
            wordcount+=1;
        }
    }
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return wordcount;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    //iterate over each bucket in hashtable
    for(int i=0; i<114; i++)
    {
        node *cursor=hashtable[i];
        while(cursor!=NULL)
        {
            //making node temp and node cursor point at the same thing
            node *temp=cursor;
            //similar to temp+=1 but this time, it's moving cursor and make temp=cursor
            cursor=cursor->next;
            free(temp);
        }
    }
    return true;
}
