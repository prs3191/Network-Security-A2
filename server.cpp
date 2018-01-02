#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<arpa/inet.h>
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
//      cout<<"\nmsq_rm:"<<msq_rm;
//      cout<<"\npowe count:"<<power_count<<"\n";
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
//                         cout<<"\npowe count:"<<power_count<<"\n";
                }
                else if(bit[i]==1){
                         if(i-1!= bit_start){
                            msq_rm = ((msq_rm)*(msq_rm))%n;
                                power_count*=2;
//                                cout<<"\nmsq_rm:"<<msq_rm;
//                cout<<"\npowe count:"<<power_count<<"\n";
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


       int sockfd,newfd,port_num;
       char sendbuf[1000],recbuf[1000];						//buffer to send and receive the message from client
       struct sockaddr_in myaddr,thieraddr;
       int numbytes;
       string sendstr,temp;
       ostringstream output_stream;							//To send very large numbers, eg:Tb

//       socklen_t sin_size;
       unsigned int sin_size;
       unsigned long long int a,b,m,p,n,Tb,Ta,Tab;

       p=12067,n=784313,m=1907;								//same input as given in question paper 
				   											//variable m = variable g given in the question paper.
				   											//variable p = variable Sa given in the question paper.
				   											//variable n = variable p given in the question paper.
       port_num=atoi(argv[1]);
       cout<<"\nEntered port number:"<<port_num;
//       cin>>port_num;
       cin.clear();											//To delete the newline character entered after executing the program. Clears the input stream
       fflush(stdin);

       sockfd=socket(AF_INET,SOCK_STREAM,0);

       myaddr.sin_family=AF_INET;
       myaddr.sin_port=htons(port_num);
       myaddr.sin_addr.s_addr=INADDR_ANY;

       memset(&myaddr.sin_zero,'\0',8);

       if( bind(sockfd,(struct sockaddr *)&myaddr,sizeof(struct sockaddr )) < 0){cout<<"\nBinding error..\n";} //Please see NOTE in README file
       else cout<<"\nBinding successful..\n";
       listen(sockfd,5);

       sin_size=sizeof(struct sockaddr_in);

       newfd=accept(sockfd,(struct sockaddr *)&thieraddr,&sin_size);	//different socket for communicating with client
       if(newfd < 0) cout<<"Error accepting connection..\n";
       else cout<<"\nConnection accepted\n";

       numbytes=recv(newfd,recbuf,999,0);					//Receive ACK from client
       recbuf[numbytes+1]='\0';
       cout<<recbuf<<"\n";
       
       sendstr="ACK from Server: Connected";
       strncpy(sendbuf,sendstr.c_str(),sizeof(sendbuf)); 	//Send ACK to client
       sendbuf[sizeof(sendbuf)-1]='\0';

       if(send(newfd, sendbuf, strlen(sendbuf)+1,0)==1) cout<<"Couldn't send ACK.";
       sendbuf[0]='\0';

      
       numbytes=recv(newfd,recbuf,999,0);					/*receive Ta from client*/
       recbuf[numbytes+1]='\0';
//       cout<<recbuf<<"\n";
     
       temp=recbuf;
       istringstream input_stream(temp);
       input_stream >> Ta;									/*m^p mod n*/

       cout<<"\nServer:\n1. Received Ta from client: "<<Ta;

       								 		 		 	   
       Tb=EXPO(m,p,n); 										/*m^p mod n*/
       output_stream << Tb;
       cout<<"\nServer:\n2. Sending  Tb to client..\n"<<Tb;

       sendstr= output_stream.str();
       strncpy(sendbuf,sendstr.c_str(),sizeof(sendbuf));	 /*send Tb to client*/
       sendbuf[sizeof(sendbuf)-1]='\0';


       if(send(newfd, sendbuf, strlen(sendbuf)+1,0)==1) cout<<"Couldn't send Tb.";
       sendbuf[0]='\0';

       /*calculate Tab*/
       Tab = EXPO(Ta,p,n); 									/*m^SaSb mod n*/
       cout<<"\nServer:\nKey is.."<<Tab<<"\n";				/*display shared secret key*/

       while(1){											/*communicate with client. Terminated when force closed*/

        numbytes=recv(newfd,recbuf,999,0);
        recbuf[numbytes+1]='\0';
       	cout<<"\nClient: "<<recbuf<<"\n";
       	recbuf[0]='\0';

		cout<<"\nServer:";
       	getline(cin,sendstr);

       	strncpy(sendbuf,sendstr.c_str(),sizeof(sendbuf));
       	sendbuf[sizeof(sendbuf)-1]=0;
       	if(send(newfd,sendbuf,strlen(sendbuf)+1,0)==1)   {
               cout<<"\nsend error\n";
               exit(1);
        	   }
        sendbuf[0]='\0';
		}
       close(newfd);
       close(sockfd);

       return 0;
}

