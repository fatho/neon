#include "idris_opts.h"
#include "idris_stats.h"
#include "idris_rts.h"

void _idris__123_runMain0_125_(VM*, VAL*);

#define VALSTACK_SIZE 1024
#define HEAP_SIZE (32*1024*1024)

VM kernel_vm;
VAL valstack[VALSTACK_SIZE];

int launch_idris_kernel() {
    char* heap1_base = (char*)(32 * 1024 * 1024);
    char* heap2_base = (char*)(heap1_base + HEAP_SIZE);

    memset(heap1_base, 0, HEAP_SIZE);
    memset(heap2_base, 0, HEAP_SIZE);
    memset(valstack, 0, sizeof(VAL)*VALSTACK_SIZE);

    VM* vm = init_vm(&kernel_vm, valstack, VALSTACK_SIZE, heap1_base, heap2_base, HEAP_SIZE, 1);
    initNullaries();
    
    _idris__123_runMain0_125_(vm, NULL);

    Stats stats = terminate(vm);

    return 0;
}
