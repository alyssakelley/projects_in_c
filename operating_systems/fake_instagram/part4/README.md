Name: Alyssa Kelley
DUCK ID: alyssak
STUDENT ID: 951480571
Project 2 - Part 4 - CIS 415 - Spring 2019
This is my own work and absolutly no code was copied. 
I did discuss this project with Anne Glickenhaus, Miguel Hernandez, and Zack Bower.


PART 4 - 

	For this part of the project, my understanding is to have the publisher() and subscriber() thread functions loop through their own files (publisher.txt and subscriber.txt). However, I have already been doing this since part 2 so my part 4 is very simular/identical to my part 2. 
 

HOW TO COMILE AND RUN: 

make
./part4.exe animal_mainfile.txt


As a re-cap, this is what the subscriber and publisher do:

	The publisher and subscriber are thread functions. They both pass in their individual thread_id for this specific thread.
	Note: I do not have seperate structs for publishers and subscriber. They are all kept in the thread_pool struct, and
	you can tell the difference based on the thread_name which is a string in the struct. This may means that you have 3 
	publishers and 2 subscribers, but your publisher id's may not be 1, 2, 3, it all depends on the order the thread is added
	in based on the mainfile.

	The first part of each function is a while loop that doesn't stop until the EXIT_P or EXIT_S (globals) depending on which function you are in (depending on if it is the publisher and subscriber) and these globals are changed to 1, and this happens after you create the publisher thread and before you join it, and same this with the subscriber. This is in order to not have the publisher function start right away and go to fast. You want to create the publisher and subscriber and then change their EXIT_P/S variables so they start closer to the same time.

	After the global EXIT_P/S variable is changed, the function will reach into the thread_elements struct and full out the filename that it is supposed to read from. In parts 3, 4 and 5, this filename is inside the mainfile, and in part 2 this filename is  argv[1], and this function is not implemented in part 1. 

	Once you open the publisher file, this function will loop through the entries 4 lines, and pull out the topic number (line 1), 
	topic url (line 2), topic caption (line 3), and the time it should sleep (line 4) and will continue this until the 
	publisher file has been completely looped through. 

	It will save the information for this newly read topic in a temporary topicentry TQ, and then passes this struct, with this
	information in it, into Enqueue to have it be added to the Topic queue (at the end of the queue). Note: The last line for the 
	publisher is the time it should sleep and this is for seeing when it expires. This is why before we enqueue, we get the time of day to keep the timestamp for this entry, and then sleep for however long line 4 for the topic says, and this way this topic will be at least that many seconds old when cleanup_thread checks to see if it is expired.

	Once you open the subscriber file, this function will loop through each line, and each line is indicating which topic it
	wants you to pull the last read entry out of. The subscriber is the viewer and in charge of making the HTML files in part 5.
	The subscriber function will call sub() which calls getentry() to see if the subscriber can see this last read entry and
	post its image.
	
	Once the publisher is done looping through the publisher file, it will increment P_EXITED (global) to keep track of all
	the publisher threads that have finished and this is good to compare to NUM_P which is the number of total publisher 	threads. This P_EXITED is not actively used, but it was to test the code while writing it, the same this happens when the subscriber is done but instead of P in all those variables, it is S (S_EXITED, NUM_S).


This is how I tested PART 4 -

1) Created a mainfile that uses all keywords.

2) The file is parsed and the topics are all created, as well as the threads. I use the keyword query a few times in my mainfile to ensure everything is being correctly added, which it is.

3) I then use the same publisher and subscriber files that I used in part 2 in this part as well.

4) At the end of this part, I print out all of the active entries for that topic and I also print how many entries have ever been part of that topic. This allows me to see that the publisher/subscriber/cleanup threads are all working. I also added more print statements to my getentry/sub that the subscriber calls to ensure that it is looping through correctly. For this part, I use fake URL's and fake captions that are easy to see what is going on. For example, topic 0 = dog, and the first entry for dog has the URL DOG0_URL and the caption is DOG0_CAPTION to see where everything is and how it was all read in.


Once again, to compile you can just type "make" and the Makefile will compile the part4.c into the part4.exe and then you can run this part with:

make
./part4.exe animal_mainfile.txt

This part was tested successfully in the virtual machine.