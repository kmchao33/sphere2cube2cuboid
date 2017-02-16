#ifndef MY_SERVER_H
#define MY_SERVER_H

#include <iostream>
#include <cstdlib>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define CONNECTION_LIMIT 1

inline void sys_exit(const char *msg) { std::cerr << msg << std::endl; exit(0); }

class MyServer {
public:
    MyServer(int _port = 5566): port(_port) {}
    //void SetPort(int _port) { port = _port; }
    void Prepare();
    void MainLoop();

    int Accept();
    void CloseClient();
    int Send(const void *buf, size_t len);
    int Recv(void *buf, size_t len);

private:
    void CreateSocket();
    void Initialize();
    void Bind();
    void Listen();
    void Communication();
    void Transformation();

    struct sockaddr_in serverAddr, clientAddr;
    int serverSockfd, clientSockfd;
    int port;
};

#endif
