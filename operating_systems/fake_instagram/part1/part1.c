/*
Authorship Statement

Name: Alyssa Kelley
DUCK ID: alyssak
STUDENT ID: 951480571
CIS 415 Project 2

This is my own work and absolutly no code was copied. I did discuss this project with
Anne Glickenhaus, Miguel Hernandez, and Zack Bower.
*/

#define maxtopics 3 
#define maxentries 20 
#define DELTA 1
#define QUACKSIZE 500
#define CAPTIONSIZE 500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>

typedef struct {
	int entrynum;
	struct timeval timestamp;
	int pubID;
	char photoUrl[QUACKSIZE];
	char photoCaption[CAPTIONSIZE];
} topicentry;

typedef struct 
{
    int num_elements; // counter for the number of elements in each queue
    topicentry Queue[maxentries];
    int max_size;
    int last_read_entry;
    int all_entries_num;
    int front;
    int back;
    pthread_mutex_t lock;

} Topic; // for every topic there will be a queue of topicentry

Topic Topics[maxtopics]; // this is the array of all the different topics 

int is_empty(Topic topic_queue)
{
	/* This function is checking to see if the topic queue is empty or not 
	If it is empty, it returns 1, if not, it returns 0.*/
	if (topic_queue.num_elements <= 0) // if new == old then this is empty
    { 
        printf("\nThe Queue is empty!\n");
        return 1;
    }
    else
    {
    	printf("\nThe Queue is NOT empty, it has %d elements in it.\n", topic_queue.num_elements);
    }
    return 0;
}

int is_full(Topic topic_queue)
{
	/*
	Checks to see if the topic queue is full for a specific topic.
	*/
	if (topic_queue.num_elements == topic_queue.max_size)
    {
        printf("\nThe Queue is full!\n");
        return 1; // bad
        //exit(EXIT_FAILURE);
    }
    else
    {
    	printf("\nThe Queue is NOT full. There are %d more entry spaces left.\n", (topic_queue.max_size - (topic_queue.num_elements)));
    }
    return 0;
}

int count_file_lines(FILE * file)
{
	/*
	This function should go through the file, and create
	a line counter (which is the number of commands
	for the program) and returns this int.
	*/
	int lines = 0; // line_count for file

	char line[1024];

	while(fgets(line, sizeof(line), file) != NULL)
	{
		lines++; // incrementing the line count until end of file
	}

	return lines;
}



void init_topics()
{
	/*
	Initializing the global Topic struct to have all it's elements set to 0/NULL.
	*/
	int i;
	for (i = 0; i < maxtopics; i++)
	{
		Topics[i].num_elements = 0;
		Topics[i].last_read_entry = 0;
		Topics[i].all_entries_num = 0;
		Topics[i].front = 0;
		Topics[i].back = 0;
		pthread_mutex_init(&Topics[i].lock, NULL);
	}
}


int Enqueue(topicentry new_entry, int topic_num)
{
	/*
	Enqueue adds a new entry to the topic queue. The newest entires get
	added on to the back like a line.
	The critial section is between the mutex locks so that deadlock
	does not occur with the mulitthreading.
	*/
	pthread_mutex_lock(&Topics[topic_num].lock);

	if (is_full(Topics[topic_num]) == 1)
	{
		pthread_mutex_unlock(&Topics[topic_num].lock);
        return 1;
	}
    // printf("Enqueing at this position in Topics Queue: %d\n", Topics[topic_num].back);
    Topics[topic_num].Queue[Topics[topic_num].back] = new_entry;
    Topics[topic_num].back ++;
    Topics[topic_num].num_elements ++;
	Topics[topic_num].all_entries_num++;

    pthread_mutex_unlock(&Topics[topic_num].lock);
    return 0;
}

void Dequeue(int topic_num)
{ 
    /*
	Dequeue removes the oldest entry from the topic queue. The oldest entiry is
	is the front of the queue like a line.
	The critial section is between the mutex locks so that deadlock
	does not occur with the mulitthreading.

	Note: When you dequeue, this is not physcially removing anything
	from the topic queue, it is just changing the number of elements
	that it says it currently has in the topic queue and changes the 
	front position to skip over this entry that is being 
	"removed".
	*/
	pthread_mutex_lock(&Topics[topic_num].lock);

	struct timeval time;
	gettimeofday(&time, NULL);
    if (Topics[topic_num].num_elements <= 0) // if new == old then this is empty
    { 
        printf("Unable to Dequeue -- The Queue is empty for Topic[%d]\n", topic_num); // Error checking to see if empty
        pthread_mutex_unlock(&Topics[topic_num].lock);
        return;
    }

    // printf("Dequeued entry #%d for Topic %d\n", Topics[topic_num].Queue[Topics[topic_num].front].entrynum, topic_num);
	Topics[topic_num].front++; // always want to increase front here to get next value.
	Topics[topic_num].num_elements--; // taking something out of the queue so decrease the size.
    pthread_mutex_unlock(&Topics[topic_num].lock);
}

int getentry(int lastentry, int topic_num, topicentry * t)
{
	/*
	Get entry uses the last read entry number and sees if that is a valid entry or not
	to read. If it is a valid entry, and the queue is not empty, then it decides
	if that entry is still in the topic queue, or if it has been dequeued already.
	If that entry is valid, and in the queue, then getentry will return 1. If not, then
	it will return the next available entry number that you can read from the topic
	queue. sub() function calls getentry and uses the returned value with the 
	subscriber() thread function.
	*/
	pthread_mutex_lock(&Topics[topic_num].lock);

	int data = -100; 

	if (is_empty(Topics[topic_num]) == 1)
	{
		data = -1;
		pthread_mutex_unlock(&Topics[topic_num].lock);
		return data;
	}

	int back_index = Topics[topic_num].back - 1;
	int back_num = Topics[topic_num].Queue[back_index].entrynum;

	int front_index = Topics[topic_num].front;
	int front_num = Topics[topic_num].Queue[front_index].entrynum;

	int i;
	for (i = front_num; i < Topics[topic_num].all_entries_num; i++)
	{
		if (Topics[topic_num].Queue[i].entrynum == lastentry)
		{
			*t = Topics[topic_num].Queue[i];
			pthread_mutex_unlock(&Topics[topic_num].lock);
			data = 1;
			printf("Here is your entry.\n");
			return data; //found
		}

		else if (Topics[topic_num].Queue[i].entrynum > lastentry)
		{
			*t = Topics[topic_num].Queue[i];
			int data = Topics[topic_num].Queue[i].entrynum;
			pthread_mutex_unlock(&Topics[topic_num].lock);
			printf("The item you were trying to read has been dequeued, the next available entry is #%d.\n", data);
			return data;
		}
	}
	if (data == -100)
	{
		printf("This is not a valid entry.\n");
		pthread_mutex_unlock(&Topics[topic_num].lock);
		return data;
	}

	pthread_mutex_unlock(&Topics[topic_num].lock);
	return data;
}


void * enqueue_the_topic(int topic_num, char URL[100], char CAPTION[100], int sleep_time)
{
	/* Helper function for part 1 to help act as a mock publisher() function to fill
	up entries in the topic queue. Calls enqueue() */
	topicentry TQ;

	TQ.entrynum = Topics[topic_num].all_entries_num;

	strcpy(TQ.photoUrl, URL);

	strcpy(TQ.photoCaption, CAPTION);

	gettimeofday(&TQ.timestamp, NULL);
	Enqueue(TQ, topic_num);
	sleep(sleep_time);
	return 0;
}

void * print_the_entry(int topic_num, int entrynum)
{
	/* 
	This function will print most of the information for a Topic entry number.
	It will tell you which topic number the entry is for, and the entry number,
	URL, caption and timestamp from when it was enqueued. This function also updated
	the last read entry number that is saved in the Topic struct. Getentry uses
	this last read entry number.
	*/
	printf("\n- - - - - This entry if for Topics[%d] - - - - -\n", topic_num);
	printf("Entrynum: %d\n", Topics[topic_num].Queue[entrynum].entrynum);
	printf("PhotoURL: %s\n", Topics[topic_num].Queue[entrynum].photoUrl);
	printf("PhotoCaption %s\n", Topics[topic_num].Queue[entrynum].photoCaption);
	printf("Timestamp: %ld\n", Topics[topic_num].Queue[entrynum].timestamp.tv_sec);
	Topics[topic_num].last_read_entry = entrynum; // keeping track of the last entry we read
	return 0;
}

void * print_the_entire_topic(int topic_num)
{
	/*
	This function loops over all of the entries in a specific topic, and prints
	all of there information by calling print_the_entry() function.
	*/
	int i;
	for(i = Topics[topic_num].front; i < Topics[topic_num].all_entries_num; i++)
	{
		print_the_entry(topic_num, i);
	}
	return 0;
}

void * fill_the_entire_topic_queue(int topic_num, int max_size)
{
	/*
	This function is a helper function for part 1. It will call the 
	enqueue_the_topic() function to completely fill up a specific
	Topic. This is helpful to test that enqueue() is working
	properly.

	In this function, I pass in fake URL and captions that have
	the iteration number attached to the end so they are unqueue 
	which further helps with makeing sure the strings are being
	saved properly in the Topic/Topicentry structs.
	*/
	int i;
	Topics[topic_num].max_size = max_size;
	for(i = 0; i < Topics[topic_num].max_size+1; i++)
	{
		char var;
		var = i + '0';

		char url[100]  = "https:/fake_url_";
		strcat(url, &var);

		char caption[100] = "Fake Caption ";
		strcat(caption, &var);

		int sleep = 1;

		int max_size = 5;

		enqueue_the_topic(topic_num, url, caption, sleep);
	}
	return 0;
}

void * fill_all_topics()
{
	/*
	This function loops through all of the maxtopics
	that we can have (global variable) and will call
	fill_the_entire_topic_queue() for each topic, and
	then it will print that entire topic by calling
	print_the_entire_topic(). This is a helper function
	for part 1, and useful to make sure that all of 
	the topics can be implemented correctly.
	*/
	int i;
	int each_topic_size = 5;
	for(i = 0; i < maxtopics; i++)
	{
		fill_the_entire_topic_queue(i, each_topic_size);
		print_the_entire_topic(i);
	}
	return 0;
}

void * dequeue_entire_topic_queue(int topic_num)
{
	/*
	This function is a helper function for part 1. It
	acts as a mock cleanup_thread() to completely 
	removed all the entires from the queue by calling
	Dequeue() until the number of elements is = 0.
	This is helpful to ensure that Dequeue is working
	properly.
	*/
	while (Topics[topic_num].num_elements != 0)
	{
		Dequeue(topic_num);
		// printf("Dequeued entry #%d for Topic %d\n", Topics[topic_num].Queue[Topics[topic_num].front].entrynum, topic_num);
	}
	return 0;
}

void * empty_all_topics()
{
	/*
	This function will loop over all of the topics
	that are possibly created, and call dequeue_entire_topic_queue()
	to ensure that everything is completely empty.
	*/
	int i;
	for(i = 0; i < maxtopics; i++)
	{
		dequeue_entire_topic_queue(i);
		printf("\n");
	}
	return 0;
}

int sub(int topic_num, int lastentry)
{
	/*
	This function calls the getentry and keeps track of what it returns.
	If it returns 1, then that means it is a valid entry and the
	last read entry is still in the topic queue and you can read/view it.
	If 1 is returned, then the last read entry is incremented      	
	so it can try to read the next one next. If found is -1, the topic queue 
	is empty, and if it is -100 then that means the last read entry is not valid
	because that number is not enqueued yet. In those two cases, you cannot view
	an entry. If it is anything other than -1, -100, or 1, this means that you
	cannot read the last read entry,  but you can read the next available 
	entry in the topic queue and that is the number that is returned. 
	*/

	int found = 0;

	topicentry t;

	found = getentry(lastentry, topic_num, &t);

	if(found == -1)
	{
		// then the last entry has not been put into the queue yet bc the
		// queue is empty
		// lastentry stays the same
	}

	else if (found == 1)
	{
		// found the entry we were looking for
		lastentry++;
		Topics[topic_num].last_read_entry = lastentry;
	}

	else if (found == -100)
	{
		// this means that they were trying to read an entry that has not been enqueued yet (not valid)
	}

	else
	{
		// means that the one we wanted was dequeued so it is returning
		// the 
		lastentry = found; // start reading for the next spot
		Topics[topic_num].last_read_entry = lastentry;
	}
	// printf("This is what sub is returning %d and it is being saved as the last read entry in the Topic as %d\n", Topics[topic_num].last_read_entry, lastentry );
	return lastentry;
}

int main(int argc, char* argv[])
{
	init_topics();

	printf("\n* * * * * Making sure that all the Queues are starting off empty. * * * * *\n");

	printf("\nTopic[0] - ");
	is_empty(Topics[0]);

	printf("\nTopic[1] - ");
	is_empty(Topics[1]);

	printf("\nTopic[2] - ");
	is_empty(Topics[2]);

	printf("\n* * * * * Filling all the entries in all of the Topic Queues * * * * *\n");
	fill_all_topics();

	printf("\n* * * * * Checking if the Queues for all Topics are full or empty. * * * * *\n");

	printf("\nThis is for Topic[0]\n");
	is_full(Topics[0]);
	is_empty(Topics[0]);

	printf("\nThis is for Topic[1]\n");
	is_full(Topics[1]);
	is_empty(Topics[1]);

	printf("\nThis is for Topic[2]\n");
	is_full(Topics[2]);
	is_empty(Topics[2]);

	printf("\n* * * * * Reading all of the entries for Topic 1 * * * * *\n");
	print_the_entire_topic(1);
	printf("This is the last topicentry we read from Topic 1 - %d\n", Topics[1].last_read_entry);

	printf("\n* * * * * Emptying all the entries in Topic Queue 1 * * * * *\n");
	dequeue_entire_topic_queue(1); // front is not at entry num 0 and back is at entry num 4

	printf("\n* * * * * Filling all the entries for Topic Queue 1 (again) * * * * *\n");
	fill_the_entire_topic_queue(1, 5); // front is not at entry num 5 and back is at entry num 9 

	printf("\n* * * * * Now deleting two entires from Topic Queue 1 * * * * *\n");
	Dequeue(1); // front is not at entry num 6 and back is at entry num 9 
	Dequeue(1); // front is not at entry num 7 and back is at entry num 9 

	printf("\n* * * * * Now trying to read #%d entry, but this entry has been dequeued * * * * *\n", Topics[1].last_read_entry);

	sub(1, Topics[1].last_read_entry);
	printf("\tThis is now the last read entry: %d.\n", Topics[1].last_read_entry);

	printf("\n* * * * * Now trying to read #%d entry, but this entry should be valid * * * * *\n", Topics[1].last_read_entry);
	sub(1, Topics[1].last_read_entry);
	printf("\tThis is now the last read entry: %d\n", Topics[1].last_read_entry);

	printf("\n* * * * * Now trying to read #%d entry, and this entry should also be valid * * * * *\n", Topics[1].last_read_entry);
	sub(1, Topics[1].last_read_entry);
	printf("\tThis is now the last read entry: %d\n", Topics[1].last_read_entry);

	printf("\n* * * * * Now trying to read #%d entry, and this entry should also be valid * * * * *\n", Topics[1].last_read_entry);
	sub(1, Topics[1].last_read_entry);
	printf("\tThis is now the last read entry: %d\n", Topics[1].last_read_entry);

	printf("\n* * * * * Now trying to read #%d entry, and this entry has not been enqueued yet should is not valid! * * * * *\n", Topics[1].last_read_entry);
	sub(1, Topics[1].last_read_entry);
	printf("\tThis is now the last read entry: %d\n", Topics[1].last_read_entry);

	printf("\n\n\n* * * * * Final reading for the current entries in Topic 1 * * * * *\n");
	print_the_entire_topic(1);
	printf("\nThis is the last topicentry we read from Topic 1 - %d\n", Topics[1].last_read_entry);
	printf("This is the front: %d and the back: %d.\nCurrently has %d entires, but %d have been added throughout.\n", Topics[1].front, Topics[1].back, Topics[1].num_elements, Topics[1].all_entries_num);

	return 0;
}

