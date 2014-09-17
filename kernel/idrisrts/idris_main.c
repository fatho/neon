#include "idris_opts.h"
#include "idris_stats.h"
#include "idris_rts.h"

void _idris__123_runMain0_125_(VM*, VAL*);

#define VALSTACK_SIZE 1024
#define HEAP_SIZE 4096000

VM kernel_vm;
VAL valstack[VALSTACK_SIZE];

int launch_idris_kernel() {
    char* heap_base = (char*)0x2000000;

    memset(heap_base, 0, HEAP_SIZE);
    memset(valstack, 0, sizeof(VAL)*VALSTACK_SIZE);

VM* vm = init_vm(&kernel_vm, valstack, VALSTACK_SIZE, heap_base, HEAP_SIZE, 1);
initNullaries();

_idris__123_runMain0_125_(vm, NULL);

Stats stats = terminate(vm);

return 0;
}
