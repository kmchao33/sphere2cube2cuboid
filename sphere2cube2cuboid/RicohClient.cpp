#include "RicohClient.h"
#ifndef RICOH_HOST
#define RICOH_HOST "140.109.23.80"
#endif
#ifndef RICOH_PORT
#define RICOH_PORT 5568
#endif

void RicohClient::CreateSocket()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
        error("Error Creating socket");
    }
}

void RicohClient::Initialize(int port, const char* addr)
{
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(addr);
}

void RicohClient::Connect()
{
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
}

void RicohClient::InitializeVariable()
{
    socket_ready = false;
    	/*
  	imageBuf = (char*)malloc(5120000 * sizeof(char));
	if (imageBuf == NULL) {
		error("Error Malloc");
	}
	*/
	socket_ready = true;
}

void RicohClient::Prepare()
{
	CreateSocket();
	Initialize(RICOH_PORT, RICOH_HOST);
	Connect();
	InitializeVariable();
}

void RicohClient::error(const char *msg)
{
    perror(msg);
    socket_destroy();
    exit(1);
}

int RicohClient::socket_recv_n(void* buffer, int size)
{
	int offset = 0;
	int remain = size;
	while (remain > 0){
		int nError = recv(sockfd, (char*)buffer + offset, remain, 0);
		/*
		if ((nError == SOCKET_ERROR) || (nError == 0)){
			cout << "Receive Error:" << GetLastErrorAsString() << endl;
			return -1;
		}
		*/
		remain -= nError;
		offset += nError;
	}
	return size;
}

void RicohClient::socket_destroy()
{
	if (socket_ready){
		close(sockfd);
		// remove buffer
		//free(imageBuf);
		socket_ready = false;
		std::cout << "Socket Destroyed" << std::endl;
	}
}
