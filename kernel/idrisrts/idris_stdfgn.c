#include "idris_stdfgn.h"
#include "idris_rts.h"
#include "idris_gc.h"
#include <base/time.h>

extern char** environ;

void putStr(char* str) {
    //printf("%s", str);
}

void* fileOpen(char* name, char* mode) {
    // FILE* f = fopen(name, mode);
    // return (void*)f;
    return NULL;
}

void fileClose(void* h) {
    // FILE* f = (FILE*)h;
    // fclose(f);
}

int fileEOF(void* h) {
  // FILE* f = (FILE*)h;
  // return feof(f);
    return NULL;
}

int fileError(void* h) {
  // FILE* f = (FILE*)h;
  // return ferror(f);
    return NULL;
}

void fputStr(void* h, char* str) {
    // FILE* f = (FILE*)h;
    // fputs(str, f);
}

int fpoll(void* h)
{
    // FILE* f = (FILE*)h;
    // fd_set x;
    // struct timeval timeout;
    // timeout.tv_sec = 1;
    // timeout.tv_usec = 0;
    // int fd = fileno(f);

    // FD_ZERO(&x);
    // FD_SET(fd, &x);

    // int r = select(fd+1, &x, 0, 0, &timeout);
    // return r;
    return NULL;
}

void* do_popen(const char* cmd, const char* mode) {
//     FILE* f = popen(cmd, mode);
// //    int d = fileno(f);
// //    fcntl(d, F_SETFL, O_NONBLOCK);
//     return f;
    return NULL;
}

int isNull(void* ptr) {
    return ptr==NULL;
}

int idris_eqPtr(void* x, void* y) {
    return x==y;
}

void* idris_stdin() {
    // return (void*)stdin;
    return NULL;
}

char* getEnvPair(int i) {
    // return *(environ + i);
    return NULL;
}

int idris_time() {
    time_t t = time(NULL);
    return (int)t;
}

void idris_forceGC(void* vm) {
   idris_gc((VM*)vm); 
}
