#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include "util/CustomSemaphore.h"

struct UdpClient {
    Semaphore *mylock;
	// udp values
	struct sockaddr_in send_addr;                                     // my socket add
	char buf[BUFSIZ + 1];                                             // the data's stored buffer
	int client_sockfd;  
	int len;

	UdpClient(int port) {
        mylock = new Semaphore(false);

    	memset(&send_addr,0,sizeof(send_addr));
    	send_addr.sin_family = AF_INET;  
    	send_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    	send_addr.sin_port = htons(port); 

    	// create socket error
    	if((client_sockfd = socket(PF_INET,SOCK_DGRAM, 0)) < 0)  
    	{    
        	perror("socket error");  
        	exit(1); 
    	}  
	}

	~UdpClient() {
		shutdown(client_sockfd, SHUT_RDWR);  
	}

	bool sendUdpPackage(int length) {
        mylock->wait();
        mylock->inlock();
    	printf("sending length: %d\n", length);
      
    	// send to 
    	if((len=sendto(client_sockfd,buf,length,0,(struct sockaddr *)&send_addr,sizeof(struct sockaddr)))<0)  
    	{
            //fprintf(stderr, "Error code:%d\n", errno);
        	perror("send error"); 
            mylock->unlock();  
        	return false;
    	}
        mylock->unlock();
    	return true;
	}

    bool sendUdpPackage(const char* message, int length) {
        memcpy(buf, message, length);
        return sendUdpPackage(length);
    }

	char *getData() {
		return buf;
	}
};

#endif