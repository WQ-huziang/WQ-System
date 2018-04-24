#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <stdlib.h>
#include <memory.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <stdio.h>
#include "util/CustomSemaphore.h"

struct UdpServer {
    Semaphore *mylock;
	// udp values
	struct sockaddr_in my_addr;                                       // my socket add
	struct sockaddr_in recv_addr;                                     // others socket add
	char buf[BUFSIZ + 1];                                             // the data's stored buffer
	int server_sockfd;  
	int len;  
	unsigned int sin_size;

	UdpServer(int port) {
        mylock = new Semaphore(false);

		memset(&my_addr,0,sizeof(my_addr)); 
   		my_addr.sin_family = AF_INET;                // IP connect
    	my_addr.sin_addr.s_addr = INADDR_ANY;        // can communicate with any inner Internet 
    	my_addr.sin_port = htons(port);              // set listen port

    	// create socket error
    	if((server_sockfd = socket(PF_INET,SOCK_DGRAM, 0)) < 0)  
    	{    
        	perror("socket error");  
        	exit(1); 
    	}  
      
    	// bind socket error
    	if (bind(server_sockfd,(struct sockaddr *)&my_addr, sizeof(struct sockaddr)) < 0)  
    	{  
        	perror("bind error");  
        	exit(1);  
    	}  
	}

	~UdpServer() {
	}

	bool recvUdpPackage(char *data) {
        mylock->wait();
        mylock->inlock();
    	sin_size = sizeof(struct sockaddr_in);  
    	printf("waiting for a packet...\n");  
      
    	// get udp package
    	if ((len=recvfrom(server_sockfd,buf,BUFSIZ,0,(struct sockaddr *)&recv_addr,&sin_size))<0)  
    	{  
        	perror("recvfrom error");
            mylock->unlock(); 
        	return false;  
    	}  

    	printf("received packet from %s:\n",inet_ntoa(recv_addr.sin_addr));  
    	buf[len]='/0';
        strcpy(data, buf);
        mylock->unlock();
    	return true;
	}
};

#endif