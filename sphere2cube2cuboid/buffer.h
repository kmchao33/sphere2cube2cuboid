#ifndef BUFFER_H
#define BUFFER_H
#define THIRTY_MB 31457280
#include <vector>
class Buffer {
    public:
        Buffer(int _size = THIRTY_MB);
        ~Buffer();
        unsigned char* data;
        unsigned int compressedSize;
        std::vector<unsigned char*> pointers;
        std::vector<int> sizes;
        std::vector<int> widths;
        std::vector<int> heights;
};
#endif
