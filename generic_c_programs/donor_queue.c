#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_SIZE 10

/*
Notes -> 5 queses: female donors, female recipients, male donors, male recipients, hospitals
*/

typedef struct
{
    int num_elements; // counter for the number of elements in each queue
    char *strings[QUEUE_SIZE];
    int front;
    int back;
} Queue;

void InitializeQueue(Queue *q)
{
    q -> num_elements = 0;
    q -> front = 0;
    q -> back = 0;
}

void Enqueue(Queue *q, char *string)
{ // see Lecture 15 Slide 16

    if (q -> num_elements+1 >= QUEUE_SIZE)
    {
        printf("Error 1\n");
        exit(EXIT_FAILURE);
    }

    q -> strings[q -> back%QUEUE_SIZE] = string;
    q -> back++;
    q -> num_elements++;
}

char * Dequeue(Queue *q)
{ 
    // Source: GeeksforGeeks 
    // https://www.geeksforgeeks.org/circular-queue-set-1-introduction-array-implementation/

    char * data = q->strings[q->front%QUEUE_SIZE]; // First in, first out! 
    if (q -> num_elements <= 0)
    { 
        printf("Error 2 -- %d\n", q -> num_elements); // Error checking to see if empty
        exit(EXIT_FAILURE);
    }

    q -> front++; // always want to increase front here to get next value.
    q -> num_elements--; // taking something out of the queue so decrease the size.
    return data;
}

void *Front(Queue *q)
{
    void * front_person;
    if (q -> num_elements == 0) //if (q -> front == -1) 
    { 
        front_person = NULL;
        //printf("There are no unmatched entries.\n");
        return front_person;
    } 
    else
    {
        front_person = q -> strings[q -> front%QUEUE_SIZE];
        return front_person;
    }
}

void PrintQueue(Queue *q)
{
    int i;
    printf("Printing queue %p\n", q);
    printf("\tThe index for the front of the queue is %d\n", q->front);
    printf("\tThe index for the back of the queue is %d\n", q->back);
    if (q->front == q->back)
    {
        printf("\tThe queue is empty.\n");
        return;
    }
    int back = q->back;
    if (q->back < q->front)
    {
        // wrapped around, so modify indexing 
        back += QUEUE_SIZE;
    }
    for (i = q->front ; i < back ; i++)
    {
        printf("\t\tEntry[%d] = \"%s\"\n", i%QUEUE_SIZE, q->strings[i%QUEUE_SIZE]);
    }
}

void
PrettyPrintQueue(Queue *q, char *type)
{
    //printf("Checking queue %s\n", type);
    if (Front(q) == NULL)
    //if (q -> num_elements == 0)
    {
        printf("No unmatched entries for %s\n", type);
    }
    else
    {
        char *s;
        printf("Unmatched %s:\n", type);
        while (q-> num_elements > 0) // Changed part of this function so it checks if not zero 
        {                            // first and then if goes to the if statement. 
            if ((s = Dequeue(q)))
            {
                printf("\t%s\n", s);
            }
        }
    }
}

char *NewString(char *s)
{
    int len = strlen(s);
    char *rv = malloc(len);
    strcpy(rv, s);
    rv[len-1] = '\0'; /* take out newline */
    return rv;
}

int main(int argc, char *argv[])
{
    // Queue q;
    // InitializeQueue(&q);
    // PrintQueue(&q);
    // Enqueue(&q, "hello");
    // PrintQueue(&q);
    // Enqueue(&q, "world");
    // PrintQueue(&q);
    // printf("Dequeue: %s\n", Dequeue(&q));
    // PrintQueue(&q);

    FILE *f_in;
    f_in = fopen(argv[1], "r");

    Queue female_donors;
    InitializeQueue(&female_donors);
    Queue female_recipients;
    InitializeQueue(&female_recipients);
    Queue male_donors;
    InitializeQueue(&male_donors);
    Queue male_recipients;
    InitializeQueue(&male_recipients);
    Queue hospitals;
    InitializeQueue(&hospitals);

    char line[256];
    char *name;
    char *hospital_name;
    int count = 0;
    while(fgets(line, 256, f_in) != NULL)
    {
        char * rv; 
        rv = NewString(line); // using the rv variable to reflect the return of Hanks NewString function.
        name = (rv + 4); // the regular names would be +4 in order to get past -> Letter:Letter:[here is 4]

        if (rv[0] == 'R')
        { // this would ideally give you the name
            if (rv[2] == 'F')
            {
                Enqueue(&female_recipients, name);
                //printf("I am enqueueing for female_recipients: %s!\n", name);
                // put name into female recip queue
            }
            if (rv[2] == 'M')
            {
                Enqueue(&male_recipients, name);
                // put name into mail recip queue
            }
            
        }
        if (rv[0] == 'D')
        {
            if (rv[2] == 'F')
            {
                Enqueue(&female_donors, name);
                // put name into female donor queue
            }
            else if (rv[2] == 'M')
            {
                Enqueue(&male_donors, name);
                // put name into mail donor queue
            }
        }
        if (rv[0] == 'H')
        {
            hospital_name = (rv +2); // You use +2 instead of +4 since Hospitals only have H: before the hospital name.
            //printf("This is the hospital name: %s \n", hospital_name);
            Enqueue(&hospitals, hospital_name);
        }

        // then if based on first part of string, then put in correct queue

        // This is error checking I was doing that is commented out:
        // printf("This is the female recip array: %s, %s, %s\n", female_recipients.strings[0], female_recipients.strings[1], female_recipients.strings[2]);
        // printf("This is the female donor array: %s, %s, %s\n", female_donors.strings[0], female_donors.strings[1], female_donors.strings[2]);
        // printf("This is the male recip array: %s, %s, %s\n", male_recipients.strings[0], male_recipients.strings[1], male_recipients.strings[2]);
        // printf("This is the male donor array: %s, %s, %s\n", male_donors.strings[0], male_donors.strings[1], male_donors.strings[2]);
        // printf("This is the hospitals array: %s, %s, %s\n", hospitals.strings[0], hospitals.strings[1], hospitals.strings[2]);

        char * donor;
        char * recip;
        char * hosp;

        if (female_donors.num_elements >= 1 && female_recipients.num_elements >= 1 && hospitals.num_elements >= 1)
        {
            donor = Dequeue(&female_donors);
            recip = Dequeue(&female_recipients);
            hosp = Dequeue(&hospitals);
            printf("MATCH: %s donates to %s at hospital %s\n", donor, recip, hosp);
        }

        else if (male_donors.num_elements >= 1 && male_recipients.num_elements >= 1 && hospitals.num_elements >= 1)
        {
            donor = Dequeue(&male_donors);
            recip = Dequeue(&male_recipients);
            hosp = Dequeue(&hospitals);
            printf("MATCH: %s donates to %s at hospital %s\n", donor, recip, hosp);
        }
        // Error checking below:
        // printf("(1) This is the size of the queue: %d\n", female_donors.num_elements);
    }
    // Error checking below:
    //printf("(2) This is the size of the queue: %d\n", female_donors.num_elements);

    PrettyPrintQueue(&female_donors, "female donors");
    //printf("Done check for female donors.\n");
    PrettyPrintQueue(&female_recipients, "female recipients");
    //printf("Done check for female recipients.\n");
    PrettyPrintQueue(&male_donors, "male donors");
    //printf("Done check for male donors.\n");
    PrettyPrintQueue(&male_recipients, "male recipients");
    //printf("Done check for male recipients.\n");
    PrettyPrintQueue(&hospitals, "hospitals");
    //printf("Done check for hospitals donors.\n");
}
 