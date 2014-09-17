#ifndef PANIC_H
#define PANIC_H

#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)

void kernel_panic(const char* msg) __attribute__ ((noreturn));

#define PANIC(msg,...) kernel_panic(msg);
#define PANICF(msg,...) { char __panic_msg[1024]; snprintf(__panic_msg, 1024, msg, __VA_ARGS__); kernel_panic(__panic_msg); }

#define assert(cond) {if(!(cond)) kernel_panic("ASSERTION FAILED\nFile: " __FILE__ "\nLine:" S__LINE__"\n " #cond);}

#endif
