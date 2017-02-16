#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <iostream>
#include <queue>
#include <mutex>

#define MQ_SIZE_LIMIT 100

struct PacketInfo {
    char *buffer;
    int length;

    PacketInfo(char* _buffer = NULL, int _length = 0): buffer(_buffer), length(_length) {}
    ~PacketInfo() {
        /* delete[] buffer; */
    }
};

class MessageQueue {
public:
    void Push(char *buf, int length)
    {
        std::cout << "Push(), current size is " << mQueue.size() << std::endl;
        std::lock_guard<std::mutex> guard(m);
        mQueue.push(new PacketInfo(buf, length));
    }

    void Pop() // need to free the memory
    {
        std::cout << "Pop(), current size is " << mQueue.size() << std::endl;
        std::lock_guard<std::mutex> guard(m);
        PacketInfo *front = mQueue.front();
        mQueue.pop();
        delete front;
    }

    unsigned Size() { return mQueue.size(); }

    PacketInfo* front() { return mQueue.front(); }
private:
    std::queue<PacketInfo*> mQueue;
    std::mutex m;
};

#endif
