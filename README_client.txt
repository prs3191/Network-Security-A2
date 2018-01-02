README.TXT for <client.cpp>

Description:
This program is run after executing server socket program in a new shell.
This is a client program in a Client-Server model using TCP sockets where the client exchanges Diffie Hellman Value with the server. Here Bob is the server and Alice is the Client.

Functions:

EXPO() - performs exponentiation of base to an exponent.
@param m - This value is unsigned long long int. This is large prime number. variable m = variable g given in the question paper.
@param p - This value is unsigned long long int. variable p = variable Sa given in the question paper.
@param n - This value is unsigned long long int. This is large prime number. variable n = variable p given in the question paper.

main() - performs socket programming. This program has client side code.
1. creates a socket.
2. Connects the socket to the specified port. This should be same as server's port number.
3. Sends ACK to the server.
4. Receives ACK from server.
5. Calculates Ta by using the EXPO function
6. Sends Ta to the server.
7. Receives Tb from the server.
8. Calculates Tab and displays in the output screen. This should be equal to Tab calculated by the server.
9. Continues interaction with the server by encrypting the message using Tab. This encryption is not required for this assignment.
10.When the connection is force terminated, the client socket is closed.

@param argv[] - Enter the port number while executing the program. The range is 0-65535. This is passed as argument to main()

To compile: Open a new shell and compile.
	 g++ client.cpp -o hw3_client_obj
To execute:
 	./hw3_client_obj 1012
	
	
NOTE:

When the connection is force terminated and if the server program and client program is run again, please use different <port_number> while executing to avoid port conflicts. 
The previously assigned port number will take time to get released.

If there's binding error while running it for 1st time, then please use different <port_number> while executing to avoid port conflicts. 
