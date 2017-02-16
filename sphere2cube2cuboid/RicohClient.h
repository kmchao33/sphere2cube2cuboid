#ifndef RICOH_CLIENT_H
#define RICOH_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <opencv/cv.h>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include "lz4.h"


class RicohClient {
public: 
	void Prepare();
	int socket_recv_n(void* buffer, int size);
	void socket_destroy();
	//void MainLoop();
    	void error(const char *msg);
	bool socket_ready;
	//char* imageBuf;
	int sockfd;
private:
	void CreateSocket();
	void Initialize(int port, const char* addr);
	void Connect();
	void InitializeVariable();
	//int sockfd;
	struct sockaddr_in serv_addr;
};
#endif
