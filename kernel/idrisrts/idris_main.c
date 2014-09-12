#include "idris_opts.h"
#include "idris_stats.h"
#include "idris_rts.h"
// The default options should give satisfactory results under many circumstances.
RTSOpts opts = { 
    .init_heap_size = 4096000,
    .max_stack_size = 4096000,
    .show_summary   = 0
};

int launch_idris_kernel() {
    VM* vm = init_vm(opts.max_stack_size, opts.init_heap_size, 1);
    initNullaries();

    _idris__123_runMain0_125_(vm, NULL);

#ifdef IDRIS_DEBUG
    if (opts.show_summary) {
        idris_gcInfo(vm, 1);
    }
#endif

    Stats stats = terminate(vm);

    if (opts.show_summary) {
        print_stats(&stats);
    }

    freeNullaries();
    return EXIT_SUCCESS;
}
