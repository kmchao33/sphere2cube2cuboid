#include "RicohClient.h"
#include "MyServer.h"
#include "mHeader.h"
#include "lz4.h"
#include "blocking_queue.h"
#include "buffer.h"

#include <pthread.h>
#include <string.h>
#include <chrono>
#include <string>


#define DEFAULT_PORT 5566
#define SERVER_ADDR "140.109.23.81"
#define HEADER_SIZE 11
#define BUFFER_SIZE 31457280

using namespace std;

BlockingQueue<Buffer*> receiveQueue;
BlockingQueue<Buffer*> s2CQueue;
BlockingQueue<Buffer*> c2CQueue;
BlockingQueue<Buffer*> compressQueue;
BlockingQueue<Buffer*> sendQueue;
auto timer = chrono::high_resolution_clock::now();

inline double getAvgTime(int count) {
    auto end = chrono::high_resolution_clock::now();
    auto total = chrono::duration_cast<chrono::milliseconds>(end-timer).count();
    return (double)total/count;
}

void* Thread_Receive(void* ptr)
{
    RicohClient ricohClient;
    ricohClient.Prepare();
    int count = 0;
    for(;;) {
        Buffer* buf = receiveQueue.pop();
        auto start = chrono::high_resolution_clock::now();
        // send request for next image
        send(ricohClient.sockfd, "1", 1, 0);
        char statusBuf[3] = {0};
        int result = recv(ricohClient.sockfd, statusBuf, sizeof(statusBuf) - 1, 0);
        if (result <=0 || statusBuf[0] != '1') {
            // no data now
            continue;
        }

        // data availble
        // 1. receive header
        char headerBuf[HEADER_SIZE + 1];
        int compressSize;
        result = recv(ricohClient.sockfd, headerBuf, HEADER_SIZE, 0);
        if (result > 0) {
            compressSize = atoi(headerBuf);
        } else {
            ricohClient.socket_destroy();
            ricohClient.error("Socket Error When Receive Header");
        }

        result = ricohClient.socket_recv_n(buf->data, compressSize);
        if (result < 0) {
            ricohClient.socket_destroy();
            ricohClient.error("Socket Error When Receive Image");
        }
        s2CQueue.push(buf);
        count++;
        auto end = chrono::high_resolution_clock::now();
#ifdef TIME_RECEIVE
        cout << "thread_receive processing time: " << chrono::duration_cast<chrono::milliseconds>(end-start).count() << "ms" << endl;
#endif
    }
    ricohClient.socket_destroy();
    return NULL;	
}

void* Thread_Sphere2Cube(void* ptr)
{
    int count = 0;
    for(;;) {
        Buffer* buf = s2CQueue.pop();
        auto start = chrono::high_resolution_clock::now();
        sphere2cube(buf->data, buf->pointers, 640, 1280, 1024);
        c2CQueue.push(buf);
        count++;
        auto end = chrono::high_resolution_clock::now();
#ifdef TIME_S2C
        cout << "thread_S2C processing time: " << chrono::duration_cast<chrono::milliseconds>(end-start).count() << "ms" << endl;
#endif
    }
    return NULL;
}

void* Thread_Cube2Cuboid(void* ptr)
{
    int count = 0;
    std::vector<cv::Mat*> c2CMats;
    for(int i = 0; i < 6; i++) {
        c2CMats.push_back(new cv::Mat(1024, 1024, CV_8UC3, NULL));
    }
    for(;;) {
        Buffer* buf = c2CQueue.pop();
        auto start = chrono::high_resolution_clock::now();
        for(int i = 0; i < 6; i++) {
            c2CMats[i]->data = buf->pointers[i];
        }
        cube2cuboid(c2CMats, buf);
        compressQueue.push(buf);
        count++;
        auto end = chrono::high_resolution_clock::now();
#ifdef TIME_C2C
        cout << "thread_C2C processing time: " << chrono::duration_cast<chrono::milliseconds>(end-start).count() << "ms" << endl;
#endif
    }
    return NULL;
}

void* Thread_Compress(void* ptr) 
{
    int count = 0;
    static const int order[6] = { 4, 5, 0, 1, 2, 3};
    char *cmpBuf = new char[BUFFER_SIZE];
    for(;;) {
        Buffer* buf = compressQueue.pop();
        auto start = chrono::high_resolution_clock::now();
        // Make packet

        // Body (Compress)
        int compressSize[6] = {0};
        int ptr = 0 + 66; // 66: header's length

        for(auto const &i : order) {
            // 3-channel to 4-channel
            cv::Mat ch3Mat(buf->heights[i], buf->widths[i], CV_8UC3, buf->pointers[i]);
            cv::Mat ch4Mat;
            cv::cvtColor(ch3Mat, ch4Mat, CV_BGR2BGRA, 4);
            // lz4 compress
            int ret = LZ4_compress_default(
                    reinterpret_cast<const char*>(ch4Mat.data),
                    cmpBuf+ptr,
                    ch4Mat.total() * ch4Mat.elemSize(),
                    BUFFER_SIZE-ptr);
            ptr += ret;
            cout << i << " ret " << ret << " ptr " << ptr << endl;
            compressSize[i] = ret;
        }
        snprintf(cmpBuf, 66, "%010d;%010d;%010d;%010d;%010d;%010d;",
                compressSize[order[0]], compressSize[order[1]], compressSize[order[2]],
                compressSize[order[3]], compressSize[order[4]], compressSize[order[5]]
                );
        memcpy(buf->data, cmpBuf, ptr);
        buf->compressedSize = ptr;
        count++;
        sendQueue.push(buf);
#ifdef TIME_COMPRESS
        auto end = chrono::high_resolution_clock::now();
        cout << "thread_Compress processing time: " << chrono::duration_cast<chrono::milliseconds>(end-start).count() << "ms" << endl;
#endif
        cout << "FPS = " << 1000/getAvgTime(count) << " count " << count << endl;
    }
}

void setTimer() {
    timer = chrono::high_resolution_clock::now();
}

void* Thread_Client(void* ptr)
{
    MyServer server(DEFAULT_PORT);
    server.Prepare();
    while(true) {
        int count = 0;
        server.Accept();
        setTimer();
        while(true) {
            Buffer* buf = sendQueue.pop();
            auto start = chrono::high_resolution_clock::now();
            int nsend = server.Send(buf->data, buf->compressedSize);
            if(nsend == -1) break;
            count++;
            receiveQueue.push(buf);

#ifdef TIME_CLIENT
            auto end = chrono::high_resolution_clock::now();
            auto duration = end - start;
            cout << "thread_Send processing time: " << chrono::duration_cast<chrono::milliseconds>(duration).count() << "ms" << endl;
#endif
#ifdef TIME_STAGE_AVG
            cout << "thread_Send average time: " << getAvgTime(count) << "ms" << endl;
#endif
            cout << "FPS = " << 1000/getAvgTime(count) << " count " << count << endl;
        }
    }
    return NULL;
}


int main(int argc, char **argv)
{
    pthread_t tid[5];
    void *ret;
    int BUFFER_POOL_SIZE = 15;
    if (argc == 2) 
        BUFFER_POOL_SIZE = atoi(argv[1]);
    // init buffer
    for (int i = 0; i < BUFFER_POOL_SIZE; i++) {
        receiveQueue.push(new Buffer(BUFFER_SIZE));
    }

    pthread_create(&tid[0], NULL, Thread_Receive, NULL);
    pthread_create(&tid[1], NULL, Thread_Sphere2Cube, NULL);
    pthread_create(&tid[2], NULL, Thread_Cube2Cuboid, NULL);
    pthread_create(&tid[3], NULL, Thread_Compress, NULL);
    pthread_create(&tid[4], NULL, Thread_Client, NULL);
    cout << "inited workers" << endl;
    pthread_join(tid[0], &ret);
    pthread_join(tid[1], &ret);
    pthread_join(tid[2], &ret);
    pthread_join(tid[3], &ret);
    pthread_join(tid[4], &ret);
    cout << "workers joined" << endl;
    return 0;
} 
