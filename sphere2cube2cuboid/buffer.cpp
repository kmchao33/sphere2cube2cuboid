#include "buffer.h"
Buffer::Buffer(int _size) {
    data = new unsigned char[_size];
    compressedSize = -1;
    for (int i = 0; i < 6; i++) {
        pointers.push_back(data + i * (_size / 6));
        sizes.push_back(-1);
        heights.push_back(-1);
        widths.push_back(-1);
    }
}

Buffer::~Buffer() {
    delete[] data;
}

