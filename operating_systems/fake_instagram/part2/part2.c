/*
Authorship Statement

Name: Alyssa Kelley
DUCK ID: alyssak
STUDENT ID: 951480571
CIS 415 Project 2

This is my own work and absolutly no code was copied. I did discuss this project with
Anne Glickenhaus, Miguel Hernandez, and Zack Bower.
*/

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

int EXIT_P = 0;
int EXIT_S = 0;
int EXIT_C = 0;

int EXIT_VAR = 0;

int ACTUAL_PROXY_AMT = 0;
int ACTUAL_TOPIC_AMT = 0;

int DELTA = 2;

typedef struct {
	int thread_id;
	pthread_mutex_t lock;
	pthread_t tid;
	char filename[100];
	char thread_type_name[100];
} thread_pool_elements;

typedef struct {
	int entrynum;
	struct timeval timestamp;
	int pubID;
	char photoUrl[QUACKSIZE];
	char photoCaption[CAPTIONSIZE];
} topicentry;

typedef struct 
{
	char topic_name[100];
	int topic_ID;
    int num_elements; // counter for the number of elements in each queue
    topicentry Queue[maxentries];
    int max_size;
    int last_read_entry;
    int latest_getentry_num;
    int all_entries_num;
    int front;
    int front_id;
    int back;
    int back_id;
    pthread_mutex_t lock;

} Topic;

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
        printf("\nThe Queue is full!\n");
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
	Initializing the thread pool elements for the thread pool struct to 0/NULL.
	*/
	int i;
	for (i = 0; i < maxtopics; i++)
	{
		Topics[i].num_elements = 0;
		Topics[i].last_read_entry = 0;
		Topics[i].latest_getentry_num = 0;
		Topics[i].all_entries_num = 0;
		Topics[i].front = 0;
		Topics[i].front_id = 0;
		Topics[i].back = 0;
		Topics[i].back_id = 0;
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
    Topics[topic_num].Queue[Topics[topic_num].back] = new_entry;


    Topics[topic_num].back ++;
    Topics[topic_num].back_id ++;
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

    if (Topics[topic_num].num_elements <= 0) // if new == old then this is empty
    { 
        printf("Unable to Dequeue -- The Queue is empty for Topic[%d]\n", topic_num); // Error checking to see if empty
        pthread_mutex_unlock(&Topics[topic_num].lock);
        return;
    }

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
		printf("Cannot read entry %d because the queue for Topic #%d is empty\n", lastentry, topic_num);
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
			printf("The subscriber found entry #%d in Topic #%d\n", lastentry, topic_num);
			return data; //found
		}

		else if (Topics[topic_num].Queue[i].entrynum > lastentry)
		{
			*t = Topics[topic_num].Queue[i];
			int data = Topics[topic_num].Queue[i].entrynum;
			pthread_mutex_unlock(&Topics[topic_num].lock);
			printf("Entry #%d has been dequeued, the next available entry for Topic #%d is #%d.\n", lastentry, topic_num, data);
			return data;
		}
	}
	if (data == -100)
	{
		printf("Entry %d is not a valid entry.\n", lastentry);
		pthread_mutex_unlock(&Topics[topic_num].lock);
		return data;
	}

	pthread_mutex_unlock(&Topics[topic_num].lock);
	return data;
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


int sub(int topic_num, int lastentry)
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
		// empty
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
		lastentry = found;
		Topics[topic_num].last_read_entry = lastentry;
	}
	return lastentry;
}


void * publisher(void * file)
{
	/*
	This function is supposed to read in the publisher_file which is formatted like:
			topic number
			url
			caption
			time to sleep
	The publisher then saves this information into a topic entry and enqueues it.
	This function is very simular to my enqueue_the_topic function from part 1.
	*/
	while(EXIT_P == 0);

	FILE * publisher_file = (FILE *) file;
	topicentry TQ;

	int line_count = count_file_lines(publisher_file);
	char line[1024];

	int topic_num = 0;
	char URL[100];
	char CAPTION[100];
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

		line_count -= 4;
	}
	printf("Done with the Publisher() function.\n");
	return 0;
}

void * cleanup_thread(void * data)
{	/*
	Cleanup checks the timestamp for the entry, and if (the current time - the timestamp) is greater than the DELTA global 	
	value, then it is dequeued because this entry is too old and has "expired". Note: I am not implementing this cleanup
	function to remove all the entires, it just continue to loop untilt he EXIT_VAR changes and publisher and subscriber are
	done, and then it stops. It is possible that there are entires left in the queue when cleanup is done. It is my understanding
	that this is how it should be implemented.
	*/
	while(EXIT_C == 0);

	int quit_cleanup = 1;
	int rt = 0;

	while(EXIT_VAR == 0)
	{
		struct timeval time;
		gettimeofday(&time, NULL);

		int i, j;
		for (i = 0; i <= maxtopics; i++)
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

			rt = check_if_all_empty();
			if (rt == 1)
			{
				quit_cleanup = 0;
			}
		}
	}
	printf("Cleanup is done now. Everything is empty.\n\n");
	return 0;
}


void * subscriber(void * file)
{
	/*
	This function is supposed to read in the subscriber_file which is just a topic number
	on each line, and calls getentry to read the next available entry for that topic. 
	The way the entries will be read is in order of oldest to newest. I know there are a
	few different descriptions on if you should get the newest or the oldest from getentry, 
	but I am implementing it as reading the next oldest entry from the last read entry.
	*/
	while(EXIT_S == 0)
	{
		// printf("Swait\t");
	}

	FILE * subscriber_file = (FILE *) file;
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

		sub(topic_num, Topics[topic_num].last_read_entry);

		line_count--;
	}
	printf("Done with the Subscriber() function.\n\n");
	return 0;
}


int main(int argc, char* argv[])
{
	init_topics(); // initializing all the topics

	FILE * pub_file, * sub_file;

	pub_file = fopen(argv[1], "r"); // reading in the publisher file
	sub_file = fopen(argv[2], "r"); // reading in the subscriber file


	if (pub_file == NULL || sub_file == NULL)
	{
		printf("Error opening files. Please try again.\n"); // ./a.out test2pub.txt test2sub.txt
		exit(1);
	}

	Topics[0].max_size = 15; // temp size for part 2 until we read mainfile in part 3 for max_size of a topic
	Topics[1].max_size = 15; // temp size for part 2 until we read mainfile in part 3 for max_size of a topic
	Topics[2].max_size = 15; // temp size for part 2 until we read mainfile in part 3 for max_size of a topic
	Topics[3].max_size = 15; // temp size for part 2 until we read mainfile in part 3 for max_size of a topic
	

	// Step 1 ) Create the publisher / subscriber threads
	pthread_t tid_P;
	pthread_t tid_P2;

	pthread_create(&tid_P, NULL, publisher, pub_file);
	pthread_create(&tid_P2, NULL, publisher, pub_file);

	printf("Done creating pub()\n");

	pthread_t tid_S;
	pthread_t tid_S2;

	pthread_create(&tid_S, NULL, subscriber, sub_file);
	pthread_create(&tid_S2, NULL, subscriber, sub_file);

	printf("Done creating sub()\n");


	// Step 2 ) Create the cleanup thread

	pthread_t tid_C;

	pthread_create(&tid_C, NULL, cleanup_thread, NULL);


	// Step 3 ) Join the pub and sub threads

	EXIT_P = 1;
	EXIT_S = 1;

	pthread_join(tid_P, NULL);
	pthread_join(tid_S, NULL);

	printf("Done joining pub() and sub()\n");


	// Step 4) Join the cleanup thread

	EXIT_C = 1;
	EXIT_VAR = 1;

	pthread_join(tid_C, NULL);
	printf("Done joining clean()\n");

	printf("Done with main!\n");

	return 0;
}
