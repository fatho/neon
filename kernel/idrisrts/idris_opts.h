#ifndef _IDRIS_OPTS_H
#define _IDRIS_OPTS_H

#include <stddef.h>

typedef struct {
    size_t init_heap_size;
    size_t max_stack_size;
    int    show_summary;
} RTSOpts;

#endif
