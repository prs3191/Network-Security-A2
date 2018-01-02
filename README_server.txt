README.TXT for <server.cpp>

Description:
This program has to be run before the socket client program.
This is a server program in a Client-Server model using TCP sockets where the server exchanges Diffie Hellman Value with the client. Here Bob is the server and Alice is the Client.

Functions:

EXPO() - performs exponentiation of base to an exponent.
@param m - This value is unsigned long long int. This is large prime number. variable m = variable g given in the question paper.
@param p - This value is unsigned long long int. variable p = variable Sb given in the question paper.
@param n - This value is unsigned long long int. This is large prime number. variable n = variable p given in the question paper.

main() - performs socket programming. This program has server side code.
1. creates a socket.
2. Binds the socket to the specified port.
3. Listens on the port and ready to accept the incoming connection
4. Accepts the incoming client
5. Receives ACK from client
6. Sends ACK to the client
7. Receives Ta from the client
8. Calculates Tb by using the EXPO function
9. Sends Tb to the client
10.Calculates Tab and displays in the output screen. This should be equal to Tab calculated by the client.
11.Continues interaction with the client by encrypting the message using Tab. This encryption is not required for this assignment.
12.When the connection is force terminated, the server socket is closed.

@param argv[] - Enter the port number while executing the program. The range is 0-65535. This is passed as argument to main()

To compile:
	 g++ server.cpp -o hw3_server_obj
To execute:
 	./hw3_server_obj 1012
	
	
NOTE:

When the connection is force terminated and if the server program and client program is run again, please use different <port_number> while executing to avoid port conflicts. 
The previously assigned port number will take time to get released.

If there's binding error while running it for 1st time, then please use different <port_number> while executing to avoid port conflicts. 
