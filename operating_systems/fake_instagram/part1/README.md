Name: Alyssa Kelley
DUCK ID: alyssak
STUDENT ID: 951480571
Project 2 - Part 1 - CIS 415 - Spring 2019
This is my own work and absolutly no code was copied. 
I did discuss this project with Anne Glickenhaus, Miguel Hernandez, and Zack Bower.


PART 1 - 
	For this part of the project, my understanding was to implement Enqueue, Dequeue and Getentry.


HOW TO COMPILE AND RUN:

make
./part1.exe


Here are my descriptions for these functions:

Enqueue(new topic entry, topic number) - 
This function takes the new topic entry and places it in the Topic[topic number] queue which is holding all of the topic entries for each individual topic. When there is a new entry, enqueue will take that entry and put it in the back of the queue. This is done by keeping track of the entrynum to show what entry it was, and then it increments the back because this entry was added to the back so now the back available space would be the one behind this newly added entry. Then number of elements and all_entries_num are also incremented in enqueue since another entry was added. num_elements keeps track of how many entries are currently avaialble in the topic queue, and the all_entries_num keeps track of all the entries that have ever been added to this queue, even if they have been removed (dequeued). If an entry is trying to be added, but the queue is full, then it will not be able to be enqueued and nothing is incremented.

Dequeue(topic number) - 
This function removes the oldest entry which will be the entry that is at the front of the queue. It first checks if the queue is empty, and if it is, there is nothing for it to remove, so it returns. If it is not empty, then it will increment the front since when this oldest one is "removed" then the second oldest one now becomes the oldest. This function also decrements the number of elements that are currently in the queue. Note: all_entries_num is not decremented because it keeps track of the number of entries that have ever been in the queue, not what is currently in there.

Getentry(last read entry, topic number, topicentry placeholder) - 
This function has been harder to determine exactly what it wants from the spec's explanation versus Professor/TA's. However, my understand especially with the help from Matt is that this function is supposed to take in the last entry that was read. The first thing this function does is see if the queue is empty. If it is empty, there is no entry that you can read so it returns -1 to indicate it is empty (Note: the project spec said to return 0, but my first entry index starts at 0 so I return -1). Then the function starts at the front entry number and loops through until all_entries_num. This allows it to go to the very end of the current queue. If the entry number for that last entry is = to the last read entry, then it is a valid entry to read, and returns 1 to indicate it's success. However, if all the entry numbers are larger than this last read entry number, then this means the entry you are trying to read has been dequeued so you cannot view that entry. Instead, it returns the next entry number you can view. There is also a variable in the Topic struct that keeps track of the last read entry, and this is updated if the last entry is found or when a different entry is found for you to read. However, this function also checks to see if the last entry is even a valid entry number. If it is not, then -100 is returned, and the last read entry variable in the struct is not updated, (also not updated if the queue is empty). It the entry is not valid, this means that entry has not been enqueued yet. 


I have other helper functions in part 1 such as: 

is_empty - 
	This function is checking is all of the topic queues are empty for all of the current topics. If they are all empty, it returns 1, if they are not all empty, return 0. cleanup_thread() calls this function, and this function calls is_empty()

is_full - 
	Checks to see if the queue for a specific topic is full or not

count_file_lines - 
	This function should go through the file, and create a line counter (which is the number of commands for the program) and returns this int.

init_topics - 
	Initializes everything to be 0, and the lock to be NULL for the Topic queue

enqueue_the_topic - 
	Helper function for part 1 to help act as a mock publisher() function to fill up entries in the topic queue. Calls enqueue() 

print_the_entry - 
	This function will print most of the information for a Topic entry number. It will tell you which topic number the entry is for, and the entry number, URL, caption and timestamp from when it was enqueued. This function also updated the last read entry number that is saved in the Topic struct. Getentry uses this last read entry number.

print_the_entire_topic - 
	This function loops over all of the entries in a specific topic, and prints all of there information by calling print_the_entry() function.

fill_the_entire_topic_queue - 
	This function is a helper function for part 1. It will call the enqueue_the_topic() function to completely fill up a specific Topic. This is helpful to test that enqueue() is working properly.

	In this function, I pass in fake URL and captions that have the iteration number attached to the end so they are enqueue 
	which further helps with making sure the strings are being saved properly in the Topic/Topicentry structs.

fill_all_topics - 
	This function loops through all of the maxtopics that we can have (global variable) and will call fill_the_entire_topic_queue() for each topic, and then it will print that entire topic by calling print_the_entire_topic(). This is a helper function for part 1, and useful to make sure that all of the topics can be implemented correctly.


dequeue_entire_topic_queue - 
	This function is a helper function for part 1. It acts as a mock cleanup_thread() to completely removed all the entires from the queue by calling Dequeue() until the number of elements is = 0. This is helpful to ensure that Dequeue is working properly. 

emptry_all_topics - 
	This function will loop over all of the topics that are possibly created, and call dequeue_entire_topic_queue() to ensure that everything is completely empty.

sub - 
	This function calls the getentry and keeps track of what it returns. If it returns 1, then that means it is a valid entry and the last read entry is still in the topic queue and you can read/view it. If 1 is returned, then the last read entry is incremented so it can try to read the next one next. If found is -1, the topic queue is empty, and if it is -100 then that means the last read entry is not valid because that number is not enqueued yet. In those two cases, you cannot view an entry. If it is anything other than -1, -100, or 1, this means that you cannot read the last read entry, but you can read the next available entry in the topic queue and that is the number that is returned. 

check_if_all_emtpy - 
	This function is checking is all of the topic queues are empty for all of the  current topics. If they are all empty, it returns 1, if they are not all empty, return 0. cleanup_thread() calls this function, and this function calls is_empty()


This is how I tested PART 1 -

1) I will be having three topic queues by setting MAXTOPICS = 3

2) I make sure that all the queues for each topic are empty

3) I fill all the Queues for each Topic

4) I then check to see if the queues are empty or full to ensure the enqueing was done correctly, and that they are all saying they are full with 5 entries in there since I set each queue to hold a max of 5 entires for testing

5) I then read all of the entries for Topic 1, this means the last entry read will be saved at the index 4 position (since I start the index for entries numbers at 0), and I also have a print statement confirming this last number that was saved.

6) I then dequeue the entire Topic 1, so everything is "removed"

7) I then re-fill the Topic 1 queue

8) I dequeue two entries from the newly filled Topic 1 queue

9) I know try to read the last read entry, but this is entrynum 4, which has been dequeued, so a print message will inform you that the entry you are trying to read has been dequeued and it will return the next available entry to view (entrynum 7)

10) I then call getentry a few more times continuing to try to read the next entry and you can see the last read entry number continues to increase until it is trying to read an entry number that is not valid because it hasn't been enqueued yet.

I have saved my output as part1_testoutput.txt in this folder as well so you can the exact output I am referring to. This testing is also hard coded in main of part 1 so when you run it, you will see the same output.

Once again, to compile you can just type "make" and the Makefile will compile the part1.c into the part1.exe and then you can run this part with:

make
./part1.exe

This part was tested successfully in the virtual machine.