#include "MockLinux.h"

namespace MockLinux {

int openReturn = 0;
int readReturn = 0;
int writeReturn = 0;
int closeReturn = 0;

int open (const char *c, int flags) {
    return openReturn;
}

size_t write (int file, const void *buffer, size_t bytes) {
    return writeReturn;
}

size_t read (int file, void *buffer, size_t bytes) {
    return readReturn;
}

int close(int file) {
    return closeReturn;
}

}
