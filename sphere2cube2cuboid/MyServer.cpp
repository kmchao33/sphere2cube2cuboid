#include "MyServer.h"

///////////////////////////////////////////////////////////
// MyServer's member function
///////////////////////////////////////////////////////////

void MyServer::CreateSocket()
{
    serverSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSockfd == -1) {
        sys_exit("[Error] Create socket failed.");
    }
}

void MyServer::Initialize()
{
    bzero( &serverAddr, sizeof(serverAddr) );
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl( INADDR_ANY );
}

void MyServer::Bind()
{
    if( bind(serverSockfd, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr_in) ) == -1 ){
        sys_exit("[Error] Bind failed.");
    }
}

void MyServer::Listen()
{
    if( listen( serverSockfd, CONNECTION_LIMIT) == -1 ){
        sys_exit("[Error] Listen failed.");
    }
}

void MyServer::Prepare()
{
    CreateSocket();
    Initialize();
    Bind();
    Listen();
}

void MyServer::MainLoop()
{
    std::cout << "MainLoop" << std::endl;
    for(;;) {
        Accept();
        //socklen_t clientAddrLen = sizeof(clientAddr);
        //clientSockfd = accept( serverSockfd, (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
        if( clientSockfd == -1 ){
            continue;
        }
        close(clientSockfd);
    }
}

int MyServer::Accept()
{
    socklen_t clientAddrLen = sizeof(clientAddr);
    int acceptSockfd = accept( serverSockfd, (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
    if( acceptSockfd == -1 ){
        //std::cerr << "Accept failed." << std::endl;
        sys_exit("[Error] Accept failed.");
    }
    clientSockfd = acceptSockfd; // TODO:
    return acceptSockfd;
}

void MyServer::CloseClient()
{
    close(clientSockfd);
}

int MyServer::Send(const void *buf, size_t len)
{
    const char *chrbuf = (const char*) buf;
    int start = 0;
    while(len>0) {
        int ret = send(clientSockfd, chrbuf + start, len, MSG_NOSIGNAL);
        len -= ret;
        start += ret;
    }
    return start;
}

int MyServer::Recv(void *buf, size_t len)
{
    return recv(clientSockfd, buf, len, 0);
}

void MyServer::Communication()
{
    send(clientSockfd, "Hello", 6, 0);
}

void MyServer::Transformation()
{
}

// TODO: Handling signal ^C

///////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////

/*

int main(int argc, char **argv)
{
    if(argc < 2) {
        std::cout << "Usage: server <port>" << std::endl;
        return 0;
    }
    int port = atoi(argv[1]);

    MyServer s(port);
    s.Prepare();
    s.MainLoop();

    return 0;
}

*/
