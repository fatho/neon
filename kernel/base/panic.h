#ifndef PANIC_H
#define PANIC_H

void kernel_panic(char* msg) __attribute__ ((noreturn));

#define PANIC(msg,...) kernel_panic(msg);

#define assert(cond) {if(cond) kernel_panic(#cond);}

#endif