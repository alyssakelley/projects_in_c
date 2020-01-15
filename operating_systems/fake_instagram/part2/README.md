Name: Alyssa Kelley
DUCK ID: alyssak
STUDENT ID: 951480571
Project 2 - Part 2 - CIS 415 - Spring 2019
This is my own work and absolutly no code was copied. 
I did discuss this project with Anne Glickenhaus, Miguel Hernandez, and Zack Bower.

PART 2 - 

	For this part of the project, my understanding was to implement publisher, subscriber, and cleanup, and to also initialize threads to execute these functions.


HOW TO COMPILE AND RUN:

make
./part2.exe animal_pub.txt animal_sub.txt


Here are my descriptions for these functions:

Publisher() -  
	This function is supposed to read in the publisher_file which is formatted like:
			topic number
			url
			caption
			time to sleep
	
	The publisher then saves this information into a topic entry and enqueues it. This function is very simular to my enqueue_the_topic function from part 1.

Subscriber() - 
	This function is supposed to read in the subscriber_file which is just a topic number on each line, and calls getentry to read the next available entry for that topic. The way the entries will be read is in order of oldest to newest. I know there are a few different descriptions on if you should get the newest or the oldest from getentry, but I am implementing it as reading the next oldest entry from the last read entry.

Cleanup() - 
	Cleanup checks the timestamp for the entry, and if (the current time - the timestamp) is greater than the DELTA global 	
	value, then it is dequeued because this entry is too old and has "expired". Note: I am not implementing this cleanup 	function to remove all the entires, it just continue to loop untilt he EXIT_VAR changes and publisher and subscriber are done, and then it stops. It is possible that there are entires left in the queue when cleanup is done. It is my understanding that this is how it should be implemented.
 

This is how I tested PART 2 -

1) I create every thread and I pass in the publisher file and the subscriber file that it will be parsing through. These two files are modified input files from those posted on canvas. 

2) Each thread function (publisher and subscriber) wait for their RUN_VAR to be updated to allow them to continue on.

3) I then join all the publisher and subscriber threads. 

4) I then change EXIT_VAR so that cleanup does not finish before publisher() and subscriber() finish. The cleanup function will continue to loop until this EXIT_VAR is changed.

5) I then join the cleanup thread so it can continue running.

I have saved my output as part2_testoutput.txt in this folder as well so you can the exact output I am referring to. This testing is also hard coded in main of part 2 so when you run it, you will see the same output. 

Once again, to compile you can just type "make" and the Makefile will compile the part2.c into the part2.exe and then you can run this part with:

make
./part2.exe animal_pub.txt animal_sub.txt

This part was tested successfully in the virtual machine.