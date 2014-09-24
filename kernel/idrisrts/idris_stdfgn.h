#ifndef _IDRISSTDFGN_H
#define _IDRISSTDFGN_H

// A collection of useful standard functions to be used by the prelude.

void putStr(char* str);
//char* readStr();

void* fileOpen(char* f, char* mode);
void fileClose(void* h);
int fileEOF(void* h);
int fileError(void* h);
void fputStr(void*h, char* str);

void* do_popen(const char* cmd, const char* mode);
int fpoll(void* h);

int idris_eqPtr(void* x, void* y);
int isNull(void* ptr);
void* idris_stdin();

char* getEnvPair(int i);

int idris_time();

void idris_forceGC();

// other primitives

#define SETMEM8(addr,val) ((*(int8_t*)(addr)) = (int8_t)(val))
#define SETMEM16(addr,val) ((*(int16_t*)(addr)) = (int16_t)(val))
#define SETMEM32(addr,val) ((*(int32_t*)(addr)) = (int32_t)(val))
#define SETMEM64(addr,val) ((*(int64_t*)(addr)) = (int64_t)(val))

#define SETARR8(base,off,val) (((int8_t*)(base))[off] = (int8_t)(val))
#define SETARR16(base,off,val) (((int16_t*)(base))[off] = (int16_t)(val))
#define SETARR32(base,off,val) (((int32_t*)(base))[off] = (int32_t)(val))
#define SETARR64(base,off,val) (((int64_t*)(base))[off] = (int64_t)(val))

#define GETMEM8(addr) (*(int8_t*)(addr))
#define GETMEM16(addr) (*(int16_t*)(addr))
#define GETMEM32(addr) (*(int32_t*)(addr))
#define GETMEM64(addr) (*(int64_t*)(addr))

#define GETARR8(base,off) ((int8_t*)(base))[off]
#define GETARR16(base,off) ((int16_t*)(base))[off]
#define GETARR32(base,off) ((int32_t*)(base))[off]
#define GETARR64(base,off) ((int64_t*)(base))[off]

#endif
