/*
 * Goal: I are compared two data structures and compared their run time to see which is more optimized.
 * Includes a Map based on linked lists, double hashing and a performance study
 * Input file is "companylist"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>

typedef struct
{
    char    *symbol;
    char    *name;
    float    lastSale;
    float    marketCap;
    int      IPOyear;
} Company;

void PrintCompany(Company *c)
{
    printf("%s:\n", c->name);
    printf("\tSymbol: %s\n", c->symbol);
    /* .2f: only print two digits after the period. */
    printf("\tLast Sale: %.2f\n", c->lastSale);
    printf("\tMarket Capitalization: %.2f\n", c->marketCap);
    printf("\tYear of Initial Public Offering: %d\n", c->IPOyear);
}

void ReadFile(const char *filename, Company **companies_rv, int *numCompanies_rv)
{
    int  i, j;

    if (filename == NULL)
    {
        fprintf(stderr, "No filename specified!\n");
        exit(EXIT_FAILURE);
    }
    FILE *f_in = fopen(filename, "r");
    if (f_in == NULL)
    {
        fprintf(stderr, "Unable to open file \"%s\"\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(f_in, 0, SEEK_END);
    int numChars = ftell(f_in);
    // printf("The number of characters is %d\n", numChars);
    fseek(f_in, 0, SEEK_SET);

    char *file_contents = malloc(sizeof(char)*numChars);
    fread(file_contents, sizeof(char), numChars, f_in);
    fclose(f_in);
    /* Note: the memory for this array is used to populate
     * the fields of the companies.  If it is freed, then
     * the company structs all become invalid.  For the
     * context of this program, this array should not be 
     * freed. */

    // Find out how many lines there are
    int numLines = 0;
    for (i = 0 ; i < numChars ; i++)
        if (file_contents[i] == '\n')
            numLines++;
    // printf("Number of lines is %d\n", numLines);

    int      numCompanies = numLines-1; // first line is header info 
    Company *companies    = malloc(sizeof(Company)*numCompanies);

    /* strtok will parse the file_contents array.  
     * The first time we call it, it will replace every '"' with '\0'.
     * It will also return the first word before a 
     */
    int numColumns = 9;
    int numberOfQuotesPerColumn = 2;
    strtok(file_contents, "\"");
    for (i = 0 ; i < numberOfQuotesPerColumn*numColumns-1 ; i++)
         strtok(NULL, "\"");
    for (i = 0 ; i < numCompanies ; i++)
    {
         companies[i].symbol = strtok(NULL, "\"");
         strtok(NULL, "\"");
         companies[i].name = strtok(NULL, "\"");
         strtok(NULL, "\"");
         companies[i].lastSale = atof(strtok(NULL, "\""));
         strtok(NULL, "\"");
         companies[i].marketCap = atof(strtok(NULL, "\""));
         strtok(NULL, "\""); 

         /* Skip ADR TSO */
         strtok(NULL, "\"");
         strtok(NULL, "\"");

         companies[i].IPOyear = atoi(strtok(NULL, "\""));
         strtok(NULL, "\"");

         /* Skip Sector, Industry, Summary Quote */
         for (j = 0 ; j < 6 ; j++)
             strtok(NULL, "\"");

         //PrintCompany(companies+i);
    }

    /* Set parameters to have output values */
    *companies_rv    = companies;
    *numCompanies_rv = numCompanies;
}

int hash(char *name, long array_size)
{
    int hashval = 0;
    char *p = name;
    while (*p != '\0')
    {
        hashval = 31*hashval + *p;
        p++;
    }

    return hashval%array_size;
}

int double_hash(char *name, long array_size)
{
    int doublehashval = 0;
    char *p = name;
    while (*p != '\0')
    {
        doublehashval = 37*doublehashval + *p;
        p++;
    }

    return 31 - doublehashval%array_size;
}

typedef struct 
{
    int         numElements;
    char      **keys; //symbol
    Company    *companies;
} MapBasedOnHashTable;

void InitializeMapBasedOnHashTable(MapBasedOnHashTable *map, int numElements)
{
    map->numElements = numElements;
    map->keys = malloc(sizeof(char *)*numElements);
    map->companies = malloc(sizeof(Company)*numElements);
    for (int i = 0 ; i < numElements ; i++)
        map->keys[i] = NULL;
}

void StoreTo_MapBasedOnHashTable(MapBasedOnHashTable *map, Company *c)
{
    int hashval = hash(c->symbol, map->numElements);
    int hashval2 = double_hash(c->symbol, map->numElements);
    for (int i = 0 ; i < map->numElements ; i++)
    {
        int idx = (hashval+i*hashval2)%(map->numElements);
        if (idx < 0) idx+= map->numElements;
        if (map->keys[idx] == NULL)
        {
            map->keys[idx]      = c->symbol;
            map->companies[idx] = *c;
            return;
        }
    }
}

Company *
FetchFrom_MapBasedOnHashTable(MapBasedOnHashTable *map, char *key)
{
    int hashval = hash(key, map->numElements);
    int hashval2 = double_hash(key, map->numElements);
    for (int i = 0 ; i < map->numElements ; i++)
    {
        int idx = (hashval+i*hashval2)%(map->numElements);
        if (idx < 0) idx+= map->numElements;
        if (map->keys[idx] == NULL)
        {
            return NULL;
        }

        if (strcmp(map->keys[idx], key) == 0)
        {
            return &(map->companies[idx]);
        }
    }
    
    return NULL;
}

void FetchAndPrint(MapBasedOnHashTable *mfht, char *key)
{
    Company *c = FetchFrom_MapBasedOnHashTable(mfht, key);
    if (c == NULL)
    {
        printf("Key %s has no corresponding company\n", key);
    }
    else
    {
        PrintCompany(c);
    }

}

// Linked lists talked about at 12:23 in the YT video from Hank.

struct ll_node
{
    Company        *companies; // value / data
    struct ll_node *next;
    int numElements;
    // key = symbol, value = company info
};

void initialize_ll(struct ll_node *cur_head, int numElements)
    {
        cur_head->numElements = numElements;
        numElements = 0;
        cur_head->companies = malloc(sizeof(Company)*numElements);
        for (int i = 0 ; i < numElements ; i++)
        {
            (cur_head->companies)->symbol = NULL;
        }
    }


struct ll_node *ll_store(Company *c, struct ll_node *cur_head)
{
    struct ll_node *rv = malloc(sizeof(struct ll_node));
    rv->companies = c; // node -> companies which is a company
    rv->next = cur_head;
    return rv;
}

struct ll_node *ll_fetch(struct ll_node *cur_head, char *key)
{
    //printf("Looking for %s!\n", key);

    if (cur_head == NULL)
        return NULL;

    /* special case for cur_head matching v */
    //printf("Comparing against %s\n", cur_head->companies->symbol);
    struct ll_node *curr = cur_head->next;
    if (strcmp(((cur_head->companies)->symbol), key) == 0)
        return cur_head;

    while (curr != NULL)
    {
        //printf("Comparing against %s\n", curr->companies->symbol);
        if (strcmp((curr->companies->symbol), key) == 0)
        {
            return curr;
        }
        curr = curr->next;
    }
    return NULL; /* nothing removed */
}

void ll_print(struct ll_node *head, char * key)
{
    struct ll_node *curr = head;

    //printf("Comparing against %s and the key %s\n", head->companies->symbol, key);

    if (head == NULL)
    {
        printf("Key %s has no corresponding company\n", key);
    }
    else
    {
        PrintCompany(head->companies);
        //head = head->next;
    }

}

int main(int argc, char *argv[])
{
    Company *companies = NULL;
    int      numCompanies;
    ReadFile(argv[1], &companies, &numCompanies);
    MapBasedOnHashTable mfht;
    printf("num companies is %d\n", numCompanies);
    InitializeMapBasedOnHashTable(&mfht, numCompanies*2);

    struct timeval startTime1;
    gettimeofday(&startTime1, 0);

    for (int i = 0 ; i < numCompanies ; i++)
    {
        StoreTo_MapBasedOnHashTable(&mfht, companies+i);
    }

    struct timeval endTime1;
    gettimeofday(&endTime1, 0);
    double seconds1 = (double)(endTime1.tv_sec - startTime1.tv_sec) +
                 (double)(endTime1.tv_usec - startTime1.tv_usec)/1000000.; 


    int counter;
    int max_counter;
    counter = 0;
    max_counter = 0;
    for (int i = 0; i < numCompanies * 2; i++)
    {
        if (mfht.keys[i] != NULL)
        {
            counter += 1;
            if(counter > max_counter)
            {
                max_counter = counter;
            }
        }
        if (mfht.keys[i] == NULL)
            counter = 0;
    }

    printf("My double hash has a max_counter of %d and it must be lower than 20!\n", max_counter);
    // This prints out 
    struct timeval startTime2;
    gettimeofday(&startTime2, 0);

    for (int i = 0; i < numCompanies; i++)
    {
        FetchFrom_MapBasedOnHashTable(&mfht, companies[i].symbol); // mfht.keys[i]);;
    }

    struct timeval endTime2;
    gettimeofday(&endTime2, 0);
    double seconds2 = (double)(endTime2.tv_sec - startTime2.tv_sec) +
                 (double)(endTime2.tv_usec - startTime2.tv_usec)/1000000.; 

    FetchAndPrint(&mfht, "ZNWAA");
    FetchAndPrint(&mfht, "Z");
    FetchAndPrint(&mfht, "ZIOP");
    FetchAndPrint(&mfht, "ZIOQ");
    FetchAndPrint(&mfht, "YIOQ");
    FetchAndPrint(&mfht, "QIOQ");
    FetchAndPrint(&mfht, "WIOQ");
    FetchAndPrint(&mfht, "XIOQ");
    FetchAndPrint(&mfht, "TIOQ");
    FetchAndPrint(&mfht, "UIOQ");
    FetchAndPrint(&mfht, "VIOQ");

    //printf("\nNow running tests for Linked Lists!\n");

    struct ll_node ll;
    struct ll_node *cur_head = NULL;
    // function header: void initialize_ll(struct ll_node *map, int numElements)
    initialize_ll(&ll, numCompanies); // don't need *2 here since a linked list is a pointer to the companies

    struct timeval startTime3;
    gettimeofday(&startTime3, 0);

    for (int i = 0 ; i < numCompanies ; i++)
    {
        cur_head = ll_store(companies+i, cur_head); // Parameters: char *v, struct ll_node *cur_head)
    }

    struct timeval endTime3;
    gettimeofday(&endTime3, 0);
    double seconds3 = (double)(endTime3.tv_sec - startTime3.tv_sec) +
                 (double)(endTime3.tv_usec - startTime3.tv_usec)/1000000.; 

    struct timeval startTime4;
    gettimeofday(&startTime4, 0);

    char * ll_current_symbol;
    for (int i = 0; i < numCompanies; i++)
    {
        ll_current_symbol = companies[i].symbol;
        ll_fetch(cur_head, ll_current_symbol);
    }

    struct timeval endTime4;
    gettimeofday(&endTime4, 0);
    double seconds4 = (double)(endTime4.tv_sec - startTime4.tv_sec) +
                 (double)(endTime4.tv_usec - startTime4.tv_usec)/1000000.; 

    ll_print(ll_fetch(cur_head, "ZNWAA"), "ZNWAA");
    ll_print(ll_fetch(cur_head, "Z"), "Z");
    ll_print(ll_fetch(cur_head, "ZIOP"), "ZIOP");
    ll_print(ll_fetch(cur_head, "ZIOQ"), "ZIOQ");
    ll_print(ll_fetch(cur_head, "YIOQ"), "YIOQ");
    ll_print(ll_fetch(cur_head, "QIOQ"), "QIOQ");
    ll_print(ll_fetch(cur_head, "WIOQ"), "WIOQ");
    ll_print(ll_fetch(cur_head, "XIOQ"), "XIOQ");
    ll_print(ll_fetch(cur_head, "TIOQ"), "TIOQ");
    ll_print(ll_fetch(cur_head, "UIOQ"), "UIOQ");
    ll_print(ll_fetch(cur_head, "VIOQ"), "VIOQ");

    printf("\nPerformance Results: \n");
    printf("This is the hashtable store time: %f\n", seconds1);
    printf("This is the linked list store time: %f\n", seconds3);
    printf("This means that linked lists is %f seconds faster for storing!\n", seconds1-seconds3);
    printf("This is the hashtable fetch and print time: %f\n", seconds2);
    printf("This is the linked list fetch time: %f\n", seconds4);
    printf("This means that linked lists is %f seconds slower for fetching!\n", seconds4-seconds2);

}

/* PERFORMANCE STUDY WRITEUP GOES HERE:

a. One or two sentences on your choice of second hash function.
    I chose my double hash function because it was simple and related very close to the original hash function, 
    with some slight variation on the prime numbers that are used in the calculation. The hope in using different 
    prime numbers compared to the first hash function was for less overlap, and this was successful. For my cluster 
    test, I am getting a count of 18 for the amount of the cluster with my double hashing. Our goal for this was to 
    have a max of 20, and since mine is under that, this is a good sign of a successful double hash function.

b. Your findings from your performance study.
    For my performance findings, I found that storing for my linked lists were fast than for the hashtables, but fetching and 
    printing is slower for linked lists. Specifically, to store the information in a hastable, this would take around 0.000460
    compared to storing it in a linked list which takes about 0.000301 which is 0.000159 faster. This makes sense since 
    storing for linked lists, is very simple, and the storing for hashtables does require some function calls to the 
    hash and double hash fucntion which will make this slower. On the other hand, fetching and printing from a hashtable 
    takes around 0.000221 whereas for a linked list it is 0.032053, and this is 0.031832 slower, and that also makes
    sense because storing it in a linked list is much more of a complex procedure compares to just putting it
    in "randomly" which does not require much computation. I would like to point out, that running this on Unix 
    is faster compared to the Linux on our virtual machines. The virtual machine gives a slower time for each store and 
    fetch, but it still does reflect the same outcome of the hashtable  store being slower than linked lists, and the 
    linked lists fetch being slower than the hashtables. 

*/