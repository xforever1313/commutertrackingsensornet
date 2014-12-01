#ifndef MOCKLINUX_H_INCLUDED
#define MOCKLINUX_H_INCLUDED

#include <cstddef>

namespace MockLinux {

extern int openReturn;
extern int readReturn;
extern int writeReturn;
extern int closeReturn;
extern void *readReturnBuffer;

int open (const char *c, int flags);
size_t write (int file, const void *buffer, size_t bytes);
size_t read (int file, void *buffer, size_t bytes);
int close(int file);

}

#endif // MOCKLINUX_H_INCLUDED
