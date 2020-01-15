Name: Alyssa Kelley
DUCK ID: alyssak
STUDENT ID: 951480571
Project 2 - Part 3 - CIS 415 - Spring 2019
This is my own work and absolutly no code was copied. 
I did discuss this project with Anne Glickenhaus, Miguel Hernandez, and Zack Bower.


PART 3 - 

	For this part of the project, my understanding was to implement a function that is parsing a mainfile which has key words in it indicating for the program to create a topic, add a thread, or query a topic or thread for more information on what is currently saved. This mainfile then starts everything running using the "start" keyword which will call another function I wrote to create the thread_pool. 
 

HOW TO COMPILE AND RUN:

make
./part3.exe animal_mainfile.txt


Here are my descriptions for these functions:

read_in_mainfile() - 
	This function reads in the mainfile that will be argv[1] when running part3. It will parse the file and look for the following keywords: create, add, query, Delta, start. 

Here is what happens when it finds each keyword:

Create - This keyword is only used when creating a topic. A topic is created with the topic number, the name of the topic, and the max_size that this topic can be. This information is saved in the static global array I have created which is Topic Topics[MAXTOPICS]. I also am keeping track of how many actual topics there are so my code can be more efficient down the line so when I am looping over my topics, I do not have to use MAXTOPICS which may be more than the number of actual topics there are. 

Add - This keyword is only used when adding a thread. The thread will be either type publisher or subscriber. The information read would be the thread_type and then file it needs to parse. This information is saved in thread_pool_elements which is a static global struct of type thread_pool Thread_Pools and inside that struct is the element array. As I did with topics, I am also keeping track of the number of actual threads so I do not have to loop over NUMPROXIES which may be a larger number than the actual number of threads created.

Query - This can be used on topics, publishers, and subscribers. It will print the information for all the topics, or all the publishers, or all the subscribers. If it is a query for topics, then it will print the topic number, the topic max_size, and the topic name. If it is a query for the publishers/subscribers, it will print the thread_type (so only subscriber or only publsiher depending on the query), and then the number thread this is, and then the filename this thread's function will be reading from. 

Delta - This will update the global DELTA value to reflect how old an entry can be before it is expired (this happens in the cleanup_thread function)

Start - This triggers the function call to start_the_thread_pool() to get everything going. 


read_stdin() - 
	This function is exactly like read_in_main() but instead of looping through a file, it takes in the standard input line by line and determines the keyword in just one line, and this is why this function is called in a while. The user needs to type CONTROL-D in order to stop the standard input stream and still allow the program functionalities to continue. If you click CONTROL-C instead, the program terminates and nothing happens.

start_the_thread_pool() - 
	This function is called when the keyword "start" is found in the mainfile read in in parts 3, 4, and 5.

	Once start is found, this means to start at the threads, and to do so, we are going to loop through ACTUAL_PROXY_AMT  	   (global) which is incremented when "add" keyword is found in the mainfile in parts 3, 4, and 5. If the thread name for that thread position is a "publisher", then it will call pthread_create for the publisher function while passing in the thread number to the publisher function so it can look in the Thread_Pools elements array to file the filename to loop through.

	The same logic is used if the thread name is "subscriber", but in this case, the subscriber thread function is passed into
	pthread_create, and the create_html_page() function is called to start the HTML files for each subscriber and each topic
	for each subscriber. 


	After all the publisher and subscriber threads have been created, then the 1 single cleanup thread is created. Up
	until this point all of the EXIT_P/S/C variables are 0 so the thread functions are being hung up on a wainting while
	loop so they so not go and finish before intended. Before we join all the publisher and subscriber threads (by calling
	the join_all_threads() function) we need to change the EXIT_P/S/C variables (all globals) to 1 so those while loops
	are not waiting to be finished anymore in the thread functions.

	After the publisher and subscriber threads are all joined, we change the EXIT_VAR to 1 so that the cleanup_thread can finish and then we join the cleanup_thread(). Once the cleanup_thread() finishes, this function is done.


join_all_threads() - 
	This function is called after the publisher and subscriber threads are created, and after the cleanup thread is created
	and right after all the EXIT_P/S/C variables are changed to 1, so all the thread functions can get past their waiting 
	while loops. This function is then called to join all of the publisher and subscriber threads. This is done by looping
	through the ACTUAL_PROXY_AMT (global) which is incremented when "add" keyword is found in the mainfile in 
	parts 3, 4, and 5.

	Then it will pull into the Thread_Pools array and go to the thread_elements struct and full out the thread name (aka
	either publisher or subscriber) and then the filename it reads from to print out a statement saying which thread is being
	joined and what file it is reading from, and it calls pthread_join on the tid that is saved in the struct. 

	Note: The tid is saved when the thread is created in start_the_thread_pool() function.

	There is a slight sleep after each thread is joined so you can see the threads starting/joining/continuing better.


I also am using two new structs for a thread pool which consists of an array of thread elements. I then have the thread_pool_elements struct which keeps track of the tid for the thread, the filename it will need to read from, and the thread_type_name. This information is read in from read_in_mainfile and used when query is requested and when looping through the NUMPROXIES global to create all the threads and join them.


This is how I tested PART 3 -

1) Created a mainfile that uses all keywords.

2) The file is parsed and the topics are all created, as well as the threads. I use the keyword query a few times in my mainfile to ensure everything is being correctly added, which it is.

3) I then use the same publisher and subscriber files that I used in part 2 in this part as well.

4) At the end of this part, I print out the front index, back index, and the number of elements that are left in the queue and you can see that each change from the beginning which shows that things are being enqueued and dequeue by the publisher and subscriber threads, and this is how I am determining it is being run correctly. 

Note: There are still elements left in the queue at the end of this part, and that is intended because the cleanup thread is finished before those last few elements have "expired".

Once again, to compile you can just type "make" and the Makefile will compile the part3.c into the part3.exe and then you can run this part with:

make
./part3.exe animal_mainfile.txt

This part was tested successfully in the virtual machine.
