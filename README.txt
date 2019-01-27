Jonathan Chang
CS 3516 - A01 18
Project 1

To compile the programs run the following commands:
	make clean
	make all
	
	These will remove all old object and executable files and then correctly build them using the makefile.
	
To run the client program:
	./http_client [-p] server_url port_number
	
	 -p 		 an optional arguement which if given will print out the RTT time.
	server_url 	 the server to contact.
	port_number	 the port number to use
	
	This client should connect to and make a simple GET request before displaying the raw html.
	
To run the server program:
	./http_server port_number
	
	port_number the port number to use
	
	This single-threaded server will wait and listen to calls and return the TMGD.html file.