/*

Authorship Statement

Name: Alyssa Kelley
DUCK ID: alyssak
STUDENT ID: 951480571
CIS 415 Project 2

This is my own work and absolutly no code was copied. I did discuss this project with
Anne Glickenhaus, Miguel Hernandez, and Zack Bower.
*/

// globals to initialize array sizes in the structs
#define maxtopics 10
#define maxentries 20
#define QUACKSIZE 500
#define CAPTIONSIZE 500
#define NUMPROXIES 10

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
#include <pthread.h> 
#include <semaphore.h> 

int P_EXITED = 0; // incremented after publisher() finishes
int NUM_P = 0; // incremented when a publisher is "add"ed from the mainfile/stdin
int S_EXITED = 0; // incremented after subscriber() finishes
int NUM_S = 0; // incremented when a subscriber is "add"ed from the mainfile/stdin

int EXIT_P = 0; // while loop waiting for the publisher function. This changed to 1 before publishers are joined
int EXIT_S = 0; // while loop waiting for the subscriber function. This changed to 1 before subscribers are joined
int EXIT_C = 0; // while loop waiting for the cleanup_thread function. This changed to 1 before sub / pub (s) are joined

int EXIT_VAR = 0; // this changes before the cleanup thread is joined

int ACTUAL_PROXY_AMT = 0;
int ACTUAL_TOPIC_AMT = 0;

int DELTA = 2; // this is just initializing the expired time and it is changed with the mainfile/stdin read in

typedef struct {
	int thread_id; // this will be the number of thread that was "add"ed
	pthread_t tid; // this is the tid created in start_the_thread_pool()
	char filename[100]; // the filename that the thread needs to be looping through
	char thread_type_name[100]; // this is the type of thread (aka publisher and subscriber)
} thread_pool_elements;


typedef struct {
	thread_pool_elements elements[NUMPROXIES]; // initialized the array of elements in the size of NUMPROXIES (global)
} thread_pool;

thread_pool Thread_Pools; // array of elements 


typedef struct {
	int entrynum; // this is always incrementing up and starts at 0 and goes up until maxentires
	struct timeval timestamp; // keeping track of timestamp before it is being enqueued 
	char photoUrl[QUACKSIZE]; // this is the caption for the URL for each image
	char photoCaption[CAPTIONSIZE]; // the caption for each image
} topicentry;

typedef struct 
{
	char topic_name[100]; // this is the topic name (example: cats)
	int topic_ID; // this is the number the topic is and this is the same as the topic_num 
	// used through the assignment and used to access the global Topics
    int num_elements; // counter for the number of elements in each queue
    topicentry Queue[maxentries]; // the queue of all the topic entires for this topic
    int max_size; // the max size, this is read in in parts 3, 4, 5 from the mainfile, otherwise just given a value in main
    int last_read_entry; // keeping track of the last read value for this topic
    int all_entries_num; // this will increment always and does not reflect the num_elements
    int front; // oldest
    int back; // newest
    pthread_mutex_t lock;

} Topic; // for every topic there will be a queue of topicentry

Topic Topics[maxtopics]; // this is the array of all the different topics 

int is_empty(Topic topic_queue)
{
	/* This function is checking to see if the topic queue is empty or not 
	If it is empty, it returns 1, if not, it returns 0.*/
	if (topic_queue.num_elements <= 0) // if new == old then this is empty
    { 
        // printf("\nThe Queue is empty!\n");
        return 1;
    }
    else
    {
    	// printf("\nThe Queue is NOT empty, it has %d elements in it.\n", topic_queue.num_elements);
    }
    return 0;
}

int check_if_all_empty()
{
	/* 
	This function is checking is all of the topic queues are empty for all of the 
	current topics. If they are all empty, it returns 1, if they are not all
	empty, return 0. cleanup_thread() calls this function, and this function
	calls is_empty()
	*/
	int i;

	int all_empty = 0;

	int empty_amt = 0;

	for(i = 0; i < maxtopics; i++)
	{
		if (is_empty(Topics[i]) == 1)
		{
			empty_amt++;
		}
	}

	if (empty_amt == maxtopics)
	{
		all_empty = 1;
	}

	return all_empty;
}

int is_full(Topic topic_queue)
{
	/*
	Checks to see if the topic queue is full for a specific topic.
	*/
	if (topic_queue.num_elements == topic_queue.max_size)
    {
        // printf("\nThe Queue is full!\n");
        return 1;
    }
    else
    {
    	// printf("\nThe Queue is NOT full. There are %d more entry spaces left.\n", (topic_queue.max_size - (topic_queue.num_elements)));
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
	int i, j;
	for (i = 0; i < maxtopics; i++)
	{
		Topics[i].num_elements = 0;
		Topics[i].last_read_entry = 0;
		Topics[i].all_entries_num = 0;
		Topics[i].front = 0;
		Topics[i].back = 0;
		pthread_mutex_init(&Topics[i].lock, NULL);
		for (j = 0; j < maxentries; j++)
		{
			strcpy(Topics[i].Queue[j].photoCaption, "NULL");
			strcpy(Topics[i].Queue[j].photoUrl, "NULL");
		}
	}
}

void init_thread_pool()
{
	/*
	Initializing the thread pool elements for the thread pool struct to 0/NULL.
	*/
	int i, j;
	for (i = 0; i < NUMPROXIES; i++)
	{
		Thread_Pools.elements[i].thread_id = 0;
		Thread_Pools.elements[i].tid = NULL;
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

    if (Topics[topic_num].num_elements <= 0)
    { 
        pthread_mutex_unlock(&Topics[topic_num].lock);
        return;
    }
	Topics[topic_num].front++; // always want to increase front here to get next value.
	Topics[topic_num].num_elements--; // taking something out of the queue so decrease the size.
    pthread_mutex_unlock(&Topics[topic_num].lock);
    return;
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
		// the queue is empty so there are no entries to view
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
			return data; // The last read entry is still in the topic queue so you can read/view it
		}

		else if (Topics[topic_num].Queue[i].entrynum > lastentry)
		{
			// this means that the last read entry has been dequeued, but data is the next entry you can read/view
			*t = Topics[topic_num].Queue[i];
			int data = Topics[topic_num].Queue[i].entrynum;
			pthread_mutex_unlock(&Topics[topic_num].lock);
			return data;
		}
	}
	if (data == -100)
	{
		// This means the last read entry is not valid, meaning it has not been enqueued yet.
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
	char url[300];
	strcpy(url, Topics[topic_num].Queue[entrynum].photoUrl);
	url[(strlen(url) - 1)] = 0;

	char caption[300];
	strcpy(caption, Topics[topic_num].Queue[entrynum].photoCaption);
	caption[(strlen(caption) - 1)] = 0;

	printf("\n- - - - - This entry if for Topics[%d] - - - - -\n", topic_num);
	printf("Entrynum: %d\n", Topics[topic_num].Queue[entrynum].entrynum);
	printf("PhotoURL: %s\n", url);
	printf("PhotoCaption %s\n", caption);
	printf("Timestamp: %ld\n", Topics[topic_num].Queue[entrynum].timestamp.tv_sec);
	Topics[topic_num].last_read_entry = entrynum; // keeping track of the last entry we read
	return 0;
}

void * print_the_entry_sub(int topic_num, int entrynum)
{
	/* 
	This function will print most of the information for a Topic entry number.
	It will tell you which topic number the entry is for, and the entry number,
	URL, caption when it is called in the sub function. This is instead of 
	printing to the HTML page, this is outputing the image information the
	subscriber is finding.
	*/
	char url[300];
	strcpy(url, Topics[topic_num].Queue[entrynum].photoUrl);
	url[(strlen(url) - 1)] = 0;

	char caption[300];
	strcpy(caption, Topics[topic_num].Queue[entrynum].photoCaption);
	caption[(strlen(caption) - 1)] = 0;

	printf("\n{Image information for Topic %s}\n", Topics[topic_num].topic_name);
	printf("\tEntrynum: %d\n", Topics[topic_num].Queue[entrynum].entrynum);
	printf("\tPhotoURL: %s\n", url);
	printf("\tPhotoCaption %s\n", caption);
	return 0;
}

void * print_the_entire_topic(int topic_num) // read the entry and keep track of it
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


int sub(int topic_num, int lastentry, int sub_num)
{
	/*
	This function calls the getentry and keeps track of what it returns. If it returns 1, then that means it
	is a valid entry and the last read entry is still in the topic queue and you can read/view it. If 1 is
	returned, then the entry is valid, and then last read entry is incremented so it can try to read the next one next.
	If found is -1, the topic queue is empty, and if it is -100 then that means the last read entry is not valid
	because that number is not enqueued yet. In those two cases, you cannot view an entry. If it is anything other
	than -1, -100, or 1, this means that you cannot read the last read entry, but you can read the next available
	entry in the topic queue and that is the number that is returned. The next last read entry is returned (but 
	also updated in the Topic struct).
	*/
	int found = 0;

	topicentry t;

	found = getentry(lastentry, topic_num, &t);

	if(found == -1)
	{
		// then the last entry has not been put into the queue yet bc the queue is empty
		// lastentry stays the same
		printf("Subscriber Thread #%d cannot find entry #%d because Topic #%d (%s) is empty.\n", sub_num, lastentry, topic_num, Topics[topic_num].topic_name);
	}

	else if (found == 1)
	{
		// found the entry we were looking for
		printf("Subscriber Thread #%d found entry #%d in Topic #%d (%s).\n", sub_num, lastentry, topic_num, Topics[topic_num].topic_name);
		if ( strcmp(Topics[topic_num].Queue[lastentry].photoUrl, "NULL") == 0)
		{
		}
		else
		{
			print_the_entry_sub(topic_num, lastentry);
		}
		lastentry++;
		Topics[topic_num].last_read_entry = lastentry;
	}

	else if (found == -100)
	{
		// this means that they were trying to read an entry that has not been enqueued yet (not valid)
		printf("Subscriber Thread #%d cannot find entry #%d because it has not been enqueued to Topic #%d (%s) yet.\n", sub_num, lastentry, topic_num, Topics[topic_num].topic_name);
	}

	else
	{
		// means that the one we wanted was dequeued so it is returning
		printf("Subscriber Thread #%d cannot find entry #%d because it has been dequeued from Topic #%d (%s), the next available entry is #%d.\n", sub_num, lastentry, topic_num, Topics[topic_num].topic_name, found);
		lastentry = found; // start reading for the next spot

		if ( strcmp(Topics[topic_num].Queue[lastentry].photoUrl, "NULL") == 0)
		{

		}
		else 
		{
			print_the_entry_sub(topic_num, lastentry);
		}
		Topics[topic_num].last_read_entry = lastentry;
	}
	return lastentry;
}

void * publisher(void * data)
{
	/*
	The publisher function is a thread function. It is passing in the thread_id for this specific publisher thread.
	Note: I do not have seperate structs for publishers and subscriber. They are all kept in the thread_pool struct, and
	you can tell the difference based on the thread_name which is a string in the struct. This may means that you have 3 
	publishers and 2 subscribers, but your publisher id's may not be 1, 2, 3, it all depends on the order the thread is added
	in based on the mainfile.

	The first part of this function is a while loop that doesn't stop until the EXIT_P (global) is changed to 1, and this happens
	after you create the publisher thread and before you join it. This is in order to not have the publisher function
	start right away and go to fast. You want to create the publisher and subscriber and then change their EXIT_P/S variables
	so they start closer to the same time.

	After the global EXIT_P variable is changed, the function will reach into the thread_elements struct and full out the filename that it is supposed to read from. 
	In parts 3, 4 and 5, this filename is inside the mainfile, and in part 2 this filename is argv[1], and this funciton is 
	not implemented in part 1. 

	Once you open the publisher file, this function will loop through the entries 4 lines, and pull out the topic number (line 1), 
	topic url (line 2), topic caption (line 3), and the time it should sleep (line 4) and will contiunue this until the 
	publisher file has been completely looped through. 

	It will save the information for this newly read topic in a temporary topicentry TQ, and then passes this struct, with this
	information in it, into Enqueue to have it be added to the Topic queue (at the end of the queue). Note: The last line for the 
	publisher is the time it should sleep and this is for seeing when it expires. This is why before we enqueue, we get the time of
	day to keep the timestamp for this entry, and then sleep for however long line 4 for the topic says, and this way this topic
	will be at least that many seconds old when cleanup_thread checks to see if it is expired.

	Once the publisher is done looping through the publisher file, it will increment P_EXITED (global) to keep track of all
	the publisher threads that have finished and this is good to compare to NUM_P which is the number of total publisher threads.
	This P_EXITED is not actively used, but it was to test the code while writing it.

	*/
	while(EXIT_P == 0);

	int thread_num = (int) data;

	FILE * publisher_file;

	publisher_file = fopen(Thread_Pools.elements[thread_num].filename, "r");

	if (publisher_file == NULL)
	{
		printf("Error opening the publisher file! Try again.\n");
		exit(1);
	}

	topicentry TQ;

	int line_count = count_file_lines(publisher_file);
	char line[1024];

	int topic_num = 0;
	int sleep;

	fseek(publisher_file, 0, SEEK_SET);

	while(line_count > 0)
	{
		TQ.entrynum = Topics[topic_num].all_entries_num;

		fgets(line, sizeof(line), publisher_file);
		sscanf(line, "%d", &topic_num);

		fgets(line, sizeof(line), publisher_file);
		strcpy(TQ.photoUrl, line);

		fgets(line, sizeof(line), publisher_file);
		strcpy(TQ.photoCaption, line);

		fgets(line, sizeof(line), publisher_file);
		sscanf(line, "%d", &sleep);

		gettimeofday(&TQ.timestamp, NULL);
		usleep(sleep * 1000000);
		Enqueue(TQ, topic_num);

		printf("\nPublisher Thread #%d finished enqueing Topics[%d] #%d entry.\n", Thread_Pools.elements[thread_num].thread_id, topic_num, TQ.entrynum);

		line_count -= 4;
	}
	P_EXITED += 1;
	printf("Done with the Publisher() function.\n");
	return 0;
}

void * cleanup_thread(void * data)
{
	/*
	The cleanup_thread function is a thread function. This thread function gets hung up on the EXIT_C condition "flag" (aka
	waiting while loop) so the cleanup thread does not finish before anything has even been enqueued. The way the threads
	are created are 1) Create the publisher / subscribers threads 2) Create the cleanup thread 3) Change the EXIT_P/S/C and
	then join the pub(s) / sub(s) 4) Change the EXIT_VAR and join the cleanup_thread, and so you want to make sure the function
	has two different condition while loops in it so they 1) the cleanup doesn't start too early and 2) the cleanup doesn't 
	finish too fast. 

	In this while loop, the cleanup_thread goes through all of the topics and then it will check if that topic is empty, and if it
	is, that is when it sleeps to allow the cleanup can catch a little break while more topic entries for this topic are supposed 
	to be enqueued by the publisher. If it is not empty, then it looks at the current timestamp, and compares this (by subtracting)
	it to the timestamp of the current entry we are looking at, and if this substracted number is greater than DELTA (global) then
	this entire is expired and it needs to be dequeued, so Dequeue is called for that topic. It is then checked to see if all
	of the topics are empty or not using the check_if_all_empty() function, and if they all are empty, then sleep is called
	for 2 seconds so the publisher can try to enqueue if it is not finished.
	*/
	while(EXIT_C == 0);

	int quit_cleanup = 1;
	int rt = 0;

	while(EXIT_VAR == 0)
	{
		int i, j;
		for (i = 0; i <= maxtopics; i++)
		{
			struct timeval time;
			gettimeofday(&time, NULL);

			if(Topics[i].num_elements == 0)
			{
				sleep(1);
			}
			else
			{

				int front_index = Topics[i].front;

				int front_num = Topics[i].Queue[front_index].entrynum;

				for (j = front_num; j < Topics[i].all_entries_num; j++)
    			{
  					long age;
					age = labs(Topics[i].Queue[j].timestamp.tv_sec - time.tv_sec);

					if (age > DELTA)
					{
						Dequeue(i);
					}
				}
			}

			rt = check_if_all_empty();
			if (rt == 1)
			{
				sleep(2);
			}
		}
	}
	printf("Cleanup is done now. Everything expired has been dequeued.\n\n");
	return 0;
}

void * subscriber(void * data)
{
	/*
	The subscriber function is a thread function. It is passing in the thread_id for this specific subsciber thread.
	Note: I do NOT have seperate structs for publishers and subscriber. They are all kept in the thread_pool struct, and
	you can tell the difference based on the thread_name which is a string in the struct. This may mean that you have 3 
	publishers and 2 subscribers, but your subscriber id's may not be 1, 2, 3, it all depends on the order the thread is added
	in based on the mainfile. For example, in my included animal_mainfile.txt, I add publisher, then two subscribers so my publisher
	has the thread_id of 0, and the subscriber id is 1 and then 2.

	The first part of this function is a while loop that doesn't stop until the EXIT_S (global) is changed to 1, and this happens
	after you create the subscriber and publisher threads and before you join them. This is in order to not have the subscriber function
	start right away and go to fast. You want to create the publisher and subscriber and then change their EXIT_P/S variables
	so they start closer to the same time. Note: I also have a sleep at the beginning of the while loop so that the subscriber
	takes a little bit longer so the publisher() has time to enqueue new entries and the cleanup_thread() has time to dequeue
	the expired entries. This way we get more interesting results.

	After the global EXIT_S variable is changed, the function will reach into the thread_elements struct and full out the filename 
	that it is supposed to read from. In parts 3, 4 and 5, this filename is inside the mainfile, and in part 2 this filename 
	is argv[2], and this funciton is not implemented in part 1. 

	Once you open the subscriber file, this function will loop through each line, and each line is indicating which topic it
	wants you to pull the last read entry out of. The subscriber is the viewer and in charge of making the HTML files in part 5.
	The subscriber function will call sub() which calls getentry() to see if the subscriber can see this last read entry and
	post its image.

	Once the subscriber is done looping through the subscriber file, it will increment S_EXITED (global) to keep track of all
	the publisher threads that have finished and this is good to compare to NUM_S which is the number of total subscriber threads.
	This S_EXITED is not actively used, but it was to test the code while writing it.

	*/
	while(EXIT_S == 0)
	{
		// printf("Swait\t");
	}

	int thread_num = (int) data;

	FILE * subscriber_file;
	subscriber_file = fopen(Thread_Pools.elements[thread_num].filename, "r");

	if (subscriber_file == NULL)
	{
		printf("Error opening the subscriber file! Try again.\n");
		exit(1);
	}

	topicentry TQ;

	int line_count = count_file_lines(subscriber_file);

	char line[1024];

	fseek(subscriber_file, 0, SEEK_SET);

	while(line_count > 0)
	{
		sleep(2);
		int topic_num = 0;

		fgets(line, sizeof(line), subscriber_file);
		sscanf(line, "%d", &topic_num);

		sub(topic_num, Topics[topic_num].last_read_entry, thread_num);

		line_count--;
	}
	int j;

	S_EXITED += 1;
	printf("Done with the Subscriber() function.\n\n");
	return 0;
}


void * join_all_threads()
{
	/*
	This function is called after the publisher and subscriber threads are created, and after the cleanup thread is created
	and right after all the EXIT_P/S/C variables are changed to 1, so all the thread functions can get past their waiting 
	while loops. This function is then called to join all of the publisher and subscriber threads. This is done by looping
	through the ACTUAL_PROXY_AMT (global) which is incremented when "add" keyword is found in the mainfile in parts 3, 4, and 5.
	Then it will pull into the Thread_Pools array and go to the thread_elements struct and full out the thread name (aka
	either publisher or subscriber) and then the filename it reads from to print out a statement saying which thread is being
	joined and what file it is reading from, and it calls pthread_join on the tid that is saved in the struct. 

	Note: The tid is saved when the thread is created in start_the_thread_pool() function.

	There is a slight sleep after each thread is joined so you can see the threads starting/joining/continuing better.
	*/
	int i;
	for(i = 0; i < ACTUAL_PROXY_AMT; i++)
	{
		printf("Joining %s reading in %s for thread #%d\n", Thread_Pools.elements[i].thread_type_name, Thread_Pools.elements[i].filename, Thread_Pools.elements[i].thread_id);
		pthread_join(Thread_Pools.elements[i].tid, NULL);
		sleep(2);
	}
	return 0;
}


void * start_the_thread_pool()
{
	/*
	This function is called when the keyword "start" is found in the mainfile read in in parts 3, 4, and 5.

	Once start is found, this means to start at the threads, and to do so, we are going to loop through ACTUAL_PROXY_AMT (global)
	which is incremented when "add" keyword is found in the mainfile in parts 3, 4, and 5. If the thread name for that thread 
	position is a "publisher", then it will call pthread_create for the publisher function while passing in the thread number
	to the publisher function so it can look in the Thread_Pools elements array to file the filename to loop through.

	The same logic is used if the thread name is "subscriber", but in this case, the subscriber thread function is passed into
	pthread_create, and the create_html_page() function is called to start the HTML files for each subscriber and each topic
	for each subscriber. 

	After all the publisher and subscriber threads have been created, then the 1 single cleanup thread is created. Up
	until this point all of the EXIT_P/S/C variables are 0 so the thread functions are being hung up on a wainting while
	loop so they so not go and finish before intended. Before we join all the publisher and subscriber threads (by calling
	the join_all_threads() function) we need to change the EXIT_P/S/C variables (all globals) to 1 so those while loops
	are not waiting to be finished anymore in the thread functions.

	After the publisher and subsciber threads are all joined, we change the EXIT_VAR to 1 so that the cleanup_thread can finish
	and then we join the cleanup_thread().

	Once the cleanup_thread() finishes, this function is done.
	*/
	printf("\n\n");
	int i;

	for(i = 0; i < ACTUAL_PROXY_AMT; i++)
	{
		char var; // each thread will be given the name of just the character it's number is
		var = i + 0;

		if (strcmp(Thread_Pools.elements[i].thread_type_name, "publisher") == 0)
		{
			long data = (long) i;
			pthread_create(&(Thread_Pools.elements[i].tid), NULL, publisher, (void *)data);
			printf("Done with creating a publisher.\n");
		}

		else if (strcmp(Thread_Pools.elements[i].thread_type_name, "subscriber") == 0)
		{
			int curr_sub_id = Thread_Pools.elements[i].thread_id;
			long data = (long) i;
			pthread_create(&(Thread_Pools.elements[i].tid), NULL, subscriber, (void *)data);
			printf("Done with creating a subscriber.\n");
		}
	}
	pthread_t C_tid;
	pthread_create(&C_tid, NULL, cleanup_thread, NULL);
	printf("Starting cleanup thread.\n");

	printf("Joining the pub/sub threads.\n");

	EXIT_P = 1;
	EXIT_S = 1;
	EXIT_C = 1;

	join_all_threads();

	printf("Starting cleanup thread.\n");
	EXIT_VAR = 1;
	pthread_join(C_tid, NULL);

	printf("Done with creating the thread pool.\n");
	return 0;
}

void * read_in_mainfile(FILE * filename)
{
	/*
	This function is the main point of part 3. It is taking in a file pointer that is 
	argv[1] and loops through this file to search for the following keywords:
	add, create, query, Delta, and start. 
	
	If the keyword "create" is found, then that means a new topic is going to be added to the Topics 
	array which is a global Topic array. This is also where ACTUAL_TOPIC_AMT (global topic counter) is
	incremented. This is good so that when we loop over all the topics in the future, we do not need
	to loop over all the maxtopics amount since this may be much larger.

	If the keyword "add" is found, you know it is going to be adding a thread. The thread will be either
	type publisher or subscriber. The information read would be the thread_type and then file it needs to parse.
	This information is saved in thread_pool_elements which is a static global struct of type thread_pool 
	Thread_Pools and inside that struct is the element array. As I did with topics, I am also keeping track
	of the number of actual threads so I do not have to loop over NUMPROXIES which may be a larger number than
	the actual number of threads created.

	If the keyword "query" is found, then you need to print out information for whatever it is asking for. Query can
	be used on topics, publishers, and subscribers. It will print the information for all the topics, or all the 
	publishers, or all the subscribers. If it is a query for topics, then it will print the topic number, 
	the topic max_size, and the topic name. If it is a query for the publishers/subscribers, it will print the 
	thread_type (so only subscriber or only publsiher depending on the query), and then the number thread this 
	is, and then the filename this thread's function will be reading from. 

	If the keyword "Delta" is found, then the global DELTA value will be updated to reflect how old an entry can
	be before it is expired (this happens in the cleanup_thread function).

	If the keyword "start" is found, then this triggers the function call to start_the_thread_pool() to get
	all the threads created, and everything going.
	*/
	FILE * pub_file, * sub_file;

	int line_ct = 0;
	line_ct = count_file_lines(filename);

	char line[1024];

	fseek(filename, 0, SEEK_SET);

	int num_topics = 0;
	int num_threads = 0;

	char request[100];
	char request2[100];
	int T_ID = 0;
	char T_name[100];
	int T_length = 0;
	char thread_type[100];
	char thread_file[100];

	while(line_ct > 0)
	{
		fgets(line, sizeof(line), filename);
		sscanf(line, "%s", request);

		if(strcmp(request, "create") == 0)
		{
			sscanf(line, "%s %s %d %s %d", request, request2, &T_ID, T_name, &T_length);
			strcpy(Topics[num_topics].topic_name, T_name);
			Topics[num_topics].max_size = T_length;

			Topics[num_topics].topic_ID = T_ID;
			num_topics++;
			ACTUAL_TOPIC_AMT++;
		}

		if(strcmp(request, "add") == 0)
		{

			sscanf(line, "%s %s %s", request, thread_type, thread_file);

			if (strcmp(thread_type, "publisher") == 0)
			{
				strcpy(Thread_Pools.elements[num_threads].thread_type_name, thread_type);
				strcpy(Thread_Pools.elements[num_threads].filename, thread_file);
				NUM_P += 1;
			}

			else if (strcmp(thread_type, "subscriber") == 0)
			{
				strcpy(Thread_Pools.elements[num_threads].thread_type_name, thread_type);
				strcpy(Thread_Pools.elements[num_threads].filename, thread_file);
				NUM_S += 1;
			}
			Thread_Pools.elements[num_threads].thread_id = num_threads;
			num_threads++;
			ACTUAL_PROXY_AMT += 1;
		}

		if(strcmp(request, "query") == 0)
		{
			sscanf(line, "%s %s", request, thread_type);
			char query_type[100];

			if (strcmp(thread_type, "publishers") == 0)
			{
				strcpy(query_type, thread_type);

			}
			if (strcmp(thread_type, "subscribers") == 0)
			{
				strcpy(query_type, thread_type);
			}

			if (strcmp(thread_type, "topics") == 0)
			{
				strcpy(query_type, thread_type);
			}

			query_type[strlen(query_type) - 1] = 0; // removing the s 

			int i;
			int thread_num = 1;

			if ((strcmp(query_type, "publisher") == 0) || (strcmp(query_type, "subscriber") == 0))
			{
				for(i = 0; i < NUMPROXIES; i++)
				{
					if (strcmp(Thread_Pools.elements[i].thread_type_name, query_type) == 0)
					{
						printf("%s thread %d reading from %s\n", Thread_Pools.elements[i].thread_type_name, thread_num, Thread_Pools.elements[i].filename);
						thread_num++;
					}
				}
			}
			else if ((strcmp(query_type, "topic") == 0))
			{
				for (i = 0; i < num_topics; i++)
				{
					printf("topic %d %d - %s\n", i, Topics[i].max_size, Topics[i].topic_name);
				}
			}
		}

		int delta_amt = 0;

		if(strcmp(request, "Delta") == 0)
		{
			sscanf(line, "%s %d", request, &delta_amt);
			DELTA = delta_amt;
		}

		int j;
		if(strcmp(request, "start") == 0)
		{
			printf("Starting!\n\n");
			start_the_thread_pool();
		}

		line_ct--;
	}
	return 0;
}

void * read_stdin(char line[1024])
{
	/*
	This function is the main point of part 3, but this function does not read through a mainfile, it 
	reads through standard input line by line. Note: You must end the standard input stream by hitting
	CNTL-D. This function will parse each standard input line, rather than looping through a file
	like readin_mainfil() does. It looks through each passed in line to search for the following keywords:
	add, create, query, Delta, and start. 
	
	If the keyword "create" is found, then that means a new topic is going to be added to the Topics 
	array which is a global Topic array. This is also where ACTUAL_TOPIC_AMT (global topic counter) is
	incremented. This is good so that when we loop over all the topics in the future, we do not need
	to loop over all the maxtopics amount since this may be much larger.

	If the keyword "add" is found, you know it is going to be adding a thread. The thread will be either
	type publisher or subscriber. The information read would be the thread_type and then file it needs to parse.
	This information is saved in thread_pool_elements which is a static global struct of type thread_pool 
	Thread_Pools and inside that struct is the element array. As I did with topics, I am also keeping track
	of the number of actual threads so I do not have to loop over NUMPROXIES which may be a larger number than
	the actual number of threads created.

	If the keyword "query" is found, then you need to print out information for whatever it is asking for. Query can
	be used on topics, publishers, and subscribers. It will print the information for all the topics, or all the 
	publishers, or all the subscribers. If it is a query for topics, then it will print the topic number, 
	the topic max_size, and the topic name. If it is a query for the publishers/subscribers, it will print the 
	thread_type (so only subscriber or only publsiher depending on the query), and then the number thread this 
	is, and then the filename this thread's function will be reading from. 

	If the keyword "Delta" is found, then the global DELTA value will be updated to reflect how old an entry can
	be before it is expired (this happens in the cleanup_thread function).

	If the keyword "start" is found, then this triggers the function call to start_the_thread_pool() to get
	all the threads created, and everything going.
	*/
	int num_topics = 0;
	int num_threads = 0;

	char request[100];
	char request2[100];
	int T_ID = 0;
	char T_name[100];
	int T_length = 0;
	char thread_type[100];
	char thread_file[100];

	sscanf(line, "%s", request);

	if(strcmp(request, "create") == 0)
		{
		sscanf(line, "%s %s %d %s %d", request, request2, &T_ID, T_name, &T_length);
		strcpy(Topics[ACTUAL_TOPIC_AMT].topic_name, T_name);
		Topics[ACTUAL_TOPIC_AMT].max_size = T_length;
		Topics[ACTUAL_TOPIC_AMT].topic_ID = T_ID;
		num_topics++;
		ACTUAL_TOPIC_AMT++;
	}

	if(strcmp(request, "add") == 0)
	{
		sscanf(line, "%s %s %s", request, thread_type, thread_file);
		if (strcmp(thread_type, "publisher") == 0)
		{
			strcpy(Thread_Pools.elements[ACTUAL_PROXY_AMT].thread_type_name, thread_type);
			strcpy(Thread_Pools.elements[ACTUAL_PROXY_AMT].filename, thread_file);
			NUM_P += 1;
		}

		else if (strcmp(thread_type, "subscriber") == 0)
		{
			strcpy(Thread_Pools.elements[ACTUAL_PROXY_AMT].thread_type_name, thread_type);
			strcpy(Thread_Pools.elements[ACTUAL_PROXY_AMT].filename, thread_file);
			NUM_S += 1;
		}

		num_threads++;
		ACTUAL_PROXY_AMT += 1;
	}

	if(strcmp(request, "query") == 0)
	{
		sscanf(line, "%s %s", request, thread_type);
		char query_type[100];

		if (strcmp(thread_type, "publishers") == 0)
		{
			strcpy(query_type, thread_type);

		}
		if (strcmp(thread_type, "subscribers") == 0)
		{
			strcpy(query_type, thread_type);
		}

		if (strcmp(thread_type, "topics") == 0)
		{
			strcpy(query_type, thread_type);
		}

		query_type[strlen(query_type) - 1] = 0; // removing the s 

		int i;
		int thread_num = 1;

		if ((strcmp(query_type, "publisher") == 0) || (strcmp(query_type, "subscriber") == 0))
		{
			for(i = 0; i < NUMPROXIES; i++)
			{
				if (strcmp(Thread_Pools.elements[i].thread_type_name, query_type) == 0)
				{
					printf("%s thread %d reading from %s\n", Thread_Pools.elements[i].thread_type_name, thread_num, Thread_Pools.elements[i].filename);
					thread_num++;
				}
			}
		}
		else if ((strcmp(query_type, "topic") == 0))
		{
			for (i = 0; i < ACTUAL_TOPIC_AMT; i++)
			{
				printf("topic %d %d - %s\n", i, Topics[i].max_size, Topics[i].topic_name);
			}
		}
	}

	int delta_amt = 0;

	if(strcmp(request, "Delta") == 0)
	{
		sscanf(line, "%s %d", request, &delta_amt);
		DELTA = delta_amt;
	}
	int j;
	if(strcmp(request, "start") == 0)
	{
		printf("Starting!\n\n");
		start_the_thread_pool();
	}

	return 0;
}

int main(int argc, char* argv[])
{
	init_topics(); // starting off my initializing 
	FILE * mainfile;
	char case_type[24]; // the type of readin for the main commands you have (aka from a file or from stdin)

	// this switch is based off my project 0 from this class
	switch(argc)
	{
		case 1: 
			mainfile = stdin;
			strcpy(case_type, "stdin");
			break;
		case 2:
			mainfile = fopen(argv[1], "r");
			strcpy(case_type, "file");
			break;

		default:
			printf("gcc <program_ex> <mainfile / stdin>\n");
			exit(1);
	}

	if (mainfile == NULL) // if there is no argv to use aka no mainfile and no stdin commands, then print an error and exit
	{
		printf("Error opening the file. Exiting program.\n\n");
		exit(1);
	}

	init_thread_pool(); // set up all the thread pool elements

	if (strcmp(case_type, "file") == 0) // if we are reading through a file, then call the read_in_mainfile() function
	{
		printf("\nReading the commands from a file.\n\n");
		read_in_mainfile(mainfile);
	}

	else if (strcmp(case_type, "stdin") == 0) // reading in line by line from standard input so call the read_stdin() function
	{
		int buffer_size = 1024;
		char input_line[buffer_size];
		printf("\nReading the commands from STDIN - click CTRL-D when done.\n\n");

		while(fgets(input_line, sizeof(input_line), mainfile) != NULL)
		{
			read_stdin(input_line);
		}
	}

	int topic;

	// finally printing what is currently in the topic queue, and how many entries it has ever had
	printf("\n\n\n*  *  *  *  *  *  *  *  *  *  Printing the Topics  *  *  *  *  *  *  *  *  *  *\n\n\n");
	for (topic = 0; topic < ACTUAL_TOPIC_AMT; topic++)
	{
		if (is_empty(Topics[topic]) == 1)
		{
			printf("\nTopic #%d is empty. It had %d entires added to it, but they have all been dequeued.\n", topic, Topics[topic].all_entries_num);
		}
		else
		{
			printf("\nTopic #%d has %d current entires. It had %d entires added to it, but some might have expired and been dequeued.\n", topic, Topics[topic].num_elements, Topics[topic].all_entries_num);
			print_the_entire_topic(topic);
		}
	}
	printf("\n\n\n");
	return 0;
}