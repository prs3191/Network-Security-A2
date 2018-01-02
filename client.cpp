#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<iostream>
#include<stdlib.h>
#include<sstream>
using namespace std;

//performs exponentiation of base to an exponent.
int EXPO(unsigned long long int m, unsigned long long int p, unsigned long long int n){
        unsigned long long int temp,power_count=0;;
        unsigned long long int msq_rm = (m*m)%n;
        int bit[64],bit_start=0;
        power_count=2;
//        cout<<"\nmsq_rm:"<<msq_rm;
//        cout<<"\npowe count:"<<power_count<<"\n";
        for(int i=63,j=0;i>=0;i--){									//process bits from MSB to LSB

                temp=(p>>i)&1;
                bit[j]=temp;
                if(bit_start==0 && temp==1) bit_start=j;
                j++;
        }
        int i=bit_start+1;
        if( bit[i]==0 )i=i+1;
        for(;i<64;i++){												//process bits from MSB to LSB and logic is same as given in textbook
            //cout<<bit[j]<<" ";
//                cout<<"\nBit at 2 power "<<63-i<<": "<<bit[i];
                if( bit[i]==0){
                         msq_rm = ((msq_rm)*(msq_rm))%n;
                         power_count*=2;
//                         cout<<"\nmsq_rm:"<<msq_rm;
//                        cout<<"\npowe count:"<<power_count<<"\n";
                }
                else if(bit[i]==1){
                         if(i-1!= bit_start){
                            msq_rm = ((msq_rm)*(msq_rm))%n;
                                power_count*=2;
//                                cout<<"\nmsq_rm:"<<msq_rm;
//               cout<<"\npowe count:"<<power_count<<"\n";
                         }
                         msq_rm = ((msq_rm)*(m))%n;
                         power_count++;
//                         cout<<"\nmsq_rm:"<<msq_rm;
//                         cout<<"\npowe count:"<<power_count<<"\n";
                }
    }
        return msq_rm;
}

int main(int argc, char *argv[])    {						//Enter the port number while executing the program. The range is 0-65535. This is passed as argument to main()

        int sockfd,numbytes,port_num;

        struct sockaddr_in thieraddr;
        char sendbuf[1000],recbuf[1000];					//buffer to send and receive the message from server
        string sendstr,temp;
        unsigned long long int a,b,m,p,n,Ta,Tb,Tab;
        ostringstream output_stream;						//To send very large numbers, eg:Ta

        m=1907;p=160011;n=784313;							//same input as given in question paper 
				   											//variable m = variable g given in the question paper.
				   											//variable p = variable Sa given in the question paper.
				   											//variable n = variable p given in the question paper.

        sockfd=socket(AF_INET,SOCK_STREAM,0);

        port_num=atoi(argv[1]);
        cout<<"\nEntered port number:"<<port_num;
//      cin>>port_num;
        cin.clear();fflush(stdin);							//To delete the newline character entered after executing the program. Clears the input stream

        thieraddr.sin_family=AF_INET;
        thieraddr.sin_port=htons(port_num);
        thieraddr.sin_addr.s_addr=INADDR_ANY;

        memset(&thieraddr.sin_zero,'\0',8);

       if( connect(sockfd,(struct sockaddr *)&thieraddr,sizeof(struct sockaddr)) < 0)
          cout <<"\nConnect error\n";

        sendstr="ACK from client: Connected";
        strncpy(sendbuf,sendstr.c_str(),sizeof(sendbuf));	
        sendbuf[sizeof(sendbuf)-1]='\0';

        if(send(sockfd, sendbuf, strlen(sendbuf)+1,0)==1) 	//Send ACK to server
              cout<<"Couldn't send ACK."; 
        sendbuf[0]='\0';

        numbytes=recv(sockfd,recbuf,999,0);					//Receive ACK from server
        recbuf[numbytes+1]='\0';

        cout<<"\n"<<recbuf<<"\n";

       
        Ta= EXPO(m,p,n); 									/*m^p mod n*/
       output_stream << Ta;
       sendstr= output_stream.str();
       strncpy(sendbuf,sendstr.c_str(),sizeof(sendbuf));
       sendbuf[sizeof(sendbuf)-1]='\0';

       cout<<"\nClient:\n1. Sending Ta to Server..\n"<<Ta;


        if(send(sockfd, sendbuf, strlen(sendbuf)+1,0)==1)  /*send Ta to server*/
		     cout<<"Couldn't send ACK";
        sendbuf[0]='\0';

       
        numbytes=recv(sockfd,recbuf,999,0);					/*receive Tb from server*/
        recbuf[numbytes+1]='\0';
//        cout<<"\n"<<recbuf<<"\n"; 
       temp=recbuf;
       istringstream input_stream(temp);
       input_stream >> Tb;									/*m^Sb mod n*/

       cout<<"\nClient:\n2. Received Tb from Server:\n"<<Tb;

	   								 		 				/*calculate Tab*/
        Tab = EXPO(Tb,p,n); 								/*g^SaSb mod n*/
        cout<<"\nClient:\nKey is.."<<Tab<<"\n";


        while(1){											/*communicate with client. Terminated when force closed*/
                cout<<"\nClient: ";
                cin.clear();fflush(stdin);
                getline(cin,sendstr);

                strncpy(sendbuf,sendstr.c_str(),sizeof(sendbuf));
                sendbuf[sizeof(sendbuf)-1]='\0';

                if(send(sockfd,sendbuf,strlen(sendbuf)+1,0)==1)
                        cout<<"\nSend error\n";

                numbytes=recv(sockfd,recbuf,999,0);

                recbuf[numbytes+1]='\0';

                cout<<"\nServer: "<<recbuf<<"\n";
                recbuf[0]='\0';
        }
                close(sockfd);
        return 0;

