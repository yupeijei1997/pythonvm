#ifndef BUFFERED_INPUT_STREAM_HPP
#define BUFFERED_INPUT_STREAM_HPP

#include <stdio.h>

#define BUFFER_LEN 1024


class BufferedInputStream {
private:
    FILE* fp;
    char szBuffer[BUFFER_LEN];
    unsigned short index;

public:
    BufferedInputStream(const char* filename) {
        fp = fopen(filename, "rb");
        fread(szBuffer, sizeof(char), BUFFER_LEN, fp);
        index = 0;
    }

    ~BufferedInputStream() {
        close();
    }

    char read() {
        if (index < BUFFER_LEN) {
            return szBuffer[index++];
        }
        else {
            index = 0;
            return szBuffer[index++];
        }
    }

    int read_int() {
        int b1 = read() & 0xff;
        int b2 = read() & 0xff;
        int b3 = read() & 0xff;
        int b4 = read() & 0xff;

        return b1 | b2 << 8 | b3 << 16 | b4 << 24;
    }

    void unread() {
        --index;
    }

    void close() {
        if (fp != NULL) {
            fclose(fp);
            fp = NULL;
        }
    }
};


#endif