Name: Alyssa Kelley
DUCK ID: alyssak
STUDENT ID: 951480571
Project 2 - Part 5 - CIS 415 - Spring 2019
This is my own work and absolutly no code was copied. 
I did discuss this project with Anne Glickenhaus, Miguel Hernandez, and Zack Bower.

PART 5 - 

	For this part of the project, my understanding is to have each subscriber thread create its own HTML page for every single
	topic. The images and captions that get added to this HTML page are coming from the subscriber thread function calling
	getentry and seeing if it is a valid entry, and if it is, post the entry to the HTML. I create an array of filenames in the thread_elements struct so that each subscriber thread has its own array of HTML files for each topic. There are 3 functions for writing to the HTML file: 1) create each file, fills in the topic image url and the image caption 3) Closes off the HTML file.

HOW TO COMILE AND RUN: 

make
./part5.exe animal_mainfile.txt

Here are the HTML functions created in part 5:

create_html_page-
	This function create a HTML file for each subscriber thread for each topic, and it saves this file in the 
	threads_elements struct (which is an array of filenames). It then will create the initial part of the HTML
	while and input the subscriber number, and then the topic name. 

	Note: the subscriber number is the number of thread in the order of which it is "add"ed. This means that the 
	subscriber number will NOT necessarily be in order depending on how everything is added. 
	In my animal_mainfile.txt I add a publisher, then subscriber 1 and subscriber 2 so they are in order, 
	but if I did add subscriber, then publisher, then subscriber, the subscriber numbers would be 0 and 2. 
	
fill_in_html_body -
	This function is called in sub() function when it is able to read a valid last entry, 
	then it will call this function to print that topic in the HTML file for that 
	subscriber. 

end_html_body -
	This function is called in main, and everything is ended by looping through all of the topics
	and all of the threads in the thread pool, and if that thread elements name is "subscriber" 
	then it will call this function with that subscriber thread_id, and the topic. 
	
	
This is how I tested PART 5 -

1) Create a publisher file that uses real images of dogs, cats, and fish, and make two different subscriber files.

2) Create a main file that will create 4 topics. Topic 0 (a topic that is not meant to ever be filled), Topic 1 (dogs), 
Topic 2 (cats), and Topic 3 (fish). 

3) I then have print statements updated what the publisher and subscriber threads are doing and what information is being added to the HTML files, and when they are being created and ended. This allows me to make sure the creation, edit, and completion of the HTML files are all in the correct order and location during execution. 

I name all of the files in this format: Topic_<topic name>_from_<sub filename>.html. I can then open the HTML files using Chrome (or Firefox on the VM) and the images that the subscriber was able to access are printed with the correct caption. I do repeat the image entries in the publisher files, so that is why there are multiple copies of the same image, but most of them have a different caption to indicate their difference.

Once again, to compile you can just type "make" and the Makefile will compile the part4.c into the part5.exe and then you can run this part with:

make
./part5.exe animal_mainfile.txt

I also added the line specifically to this Makefile to remove all the .html files after you view them. 
You can do so by:

make remove_html

All of the Makefiles also have the remove all executables command with:

make clean

This part was tested successfully in the virtual machine.
