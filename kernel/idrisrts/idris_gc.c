#include "idris_rts.h"
#include "idris_gc.h"
#include "idris_bitstring.h"
#include <base/panic.h>

/*
VAL copy(VM* vm, VAL x) {
    int i, ar;
    Closure* cl = NULL;
    if (x==NULL || ISINT(x)) {
        return x;
    }
    switch(GETTY(x)) {
    case CON:
        ar = CARITY(x);
        if (ar == 0 && CTAG(x) < 256) {
            return x;
        } else {
            allocCon(cl, vm, CTAG(x), ar, 1);
            for(i = 0; i < ar; ++i) {
    //            *argptr = copy(vm, *((VAL*)(x->info.c.args)+i)); // recursive version
                cl->info.c.args[i] = x->info.c.args[i];
            }
        }
        break;
    case FLOAT:
        cl = MKFLOATc(vm, x->info.f);
        break;
    case STRING:
        cl = MKSTRc(vm, x->info.str);
        break;
    case STROFFSET:
        cl = MKSTROFFc(vm, x->info.str_offset);
        break;
    case BUFFER:
        cl = MKBUFFERc(vm, x->info.buf);
        break;
#ifdef IDRIS_BIGINT
    case BIGINT:
        cl = MKBIGMc(vm, x->info.ptr);
        break;
#endif
    case PTR:
        cl = MKPTRc(vm, x->info.ptr);
        break;
    case MANAGEDPTR:
        cl = MKMPTRc(vm, x->info.mptr->data, x->info.mptr->size);
        break;
    case BITS8:
        cl = idris_b8CopyForGC(vm, x);
        break;
    case BITS16:
        cl = idris_b16CopyForGC(vm, x);
        break;
    case BITS32:
        cl = idris_b32CopyForGC(vm, x);
        break;
    case BITS64:
        cl = idris_b64CopyForGC(vm, x);
        break;
    case FWD:
        return x->info.ptr;
    default:
        break;
    }
    SETTY(x, FWD);
    x->info.ptr = cl;
    return cl;
}

void cheney(VM *vm) {
    int i;
    int ar;
    char* scan = vm->heap.heap;
  
    while(scan < vm->heap.next) {
       size_t inc = *((size_t*)scan);
       VAL heap_item = (VAL)(scan+sizeof(size_t));
       // If it's a CON or STROFFSET, copy its arguments
       switch(GETTY(heap_item)) {
       case CON:
           ar = ARITY(heap_item);
           for(i = 0; i < ar; ++i) {
               // printf("Copying %d %p\n", heap_item->info.c.tag, *argptr);
               VAL newptr = copy(vm, heap_item->info.c.args[i]);
               // printf("Got %p\t\t%p %p\n", newptr, scan, vm->heap_next);
               heap_item->info.c.args[i] = newptr;
           }
           break;
       case STROFFSET:
           heap_item->info.str_offset->str 
               = copy(vm, heap_item->info.str_offset->str);
           break;
       default: // Nothing to copy
           break;
       }
       scan += inc;
    }
    assert(scan == vm->heap.next);
}
*/

typedef struct {
    uintptr_t brk;
    uint32_t diff;
} brktbl_e;

int marked_alive;

brktbl_e* brktbl_partition(brktbl_e* bot, brktbl_e* top) {
    brktbl_e* pivot = top - 1;
    brktbl_e* store = bot;
    brktbl_e* i;
    brktbl_e tmp;
    for(i = bot; i < top - 1; i++) {
	if(i->brk < pivot->brk) {
	    tmp = *i;
	    *i = *store;
	    *store = tmp;
	    store++;
	}
    }
    tmp = *store;
    *store = *(top - 1);
    *(top-1) = tmp;
    return store;
}

void brktbl_sort(brktbl_e* bot, brktbl_e* top) {
    while(bot < top) {
	brktbl_e* mid = brktbl_partition(bot, top);
	if(mid - bot >= top - mid) {
	    brktbl_sort(mid, top);
	    top = mid;
	} else {
	    brktbl_sort(bot, mid);
	    bot = mid;
	}
    }
}

void compact(VM* vm) {
    brktbl_e* breakTableBot = NULL;
    brktbl_e* breakTableTop = NULL;
    
    int i;
    int ar;
    char* scan = vm->heap.heap;
    char* free = vm->heap.heap;
    char* scan2;

    int total = 0;
    int alive = 0;
  
    while(scan < vm->heap.next) {
       size_t item_size = *((size_t*)scan);
       char* item_base = scan;
       VAL heap_item = (VAL)(scan+sizeof(size_t));
       total += 1;

       if(GETGCI(heap_item)) {
	   assert(item_base >= free);
	   if(item_base == free) {
	       // contiguous block
	       // next item
	       scan += item_size;
	       free = scan;
	   } else if(item_base > free) {
	       // skipped hole, find end of next hole
	       scan2 = scan;
	       char* block_end = 0;
	       char* hole_end = 0;

	       while(scan2 < vm->heap.next) {
		   size_t next_size = *((size_t*)scan);
		   VAL next_item = (VAL)(scan2 + sizeof(size_t));
		   if(block_end != NULL && GETGCI(next_item)) {
		       // next hole ends there
		       hole_end = scan2;
		   } else if(block_end == NULL && !GETGCI(next_item)) {
		       // current block ends there
		       block_end = scan2;
		   }
		   scan2 += next_size;
	       }
	       // end of heap reached while searching for end of block
	       if(block_end == NULL) {
		   block_end = vm->heap.next;
	       }
	       // end of heap reached while searching for end of hole
	       if(hole_end == NULL) {
		   hole_end = block_end;
	       }
	       size_t block_size = block_end - scan;

	       if(breakTableBot != NULL) {
		   // we already have a break table
		   if((uintptr_t)free + block_size > (uintptr_t)breakTableBot) {
		       // move as much as possible
		       size_t n = (uintptr_t)breakTableBot - (uintptr_t)free;
		       memmove(free, scan, n);
		       free += n;
		       scan += n;
		       // roll the table
		       brktbl_e* newBrkBot = (brktbl_e*)hole_end;
		       brktbl_e tmp;
		       size_t nblocks = (block_size - n) / sizeof(tmp);
		       size_t nlast   = block_size - n - nblocks * sizeof(tmp);
		       // copy in blocks
		       for(int i = 0; i < nblocks; i++) {
			   // save break table entry
			   memcpy(&tmp, free, sizeof(tmp));
			   // copy data
			   memmove(free, scan, sizeof(tmp));
			   // copy break table entry
			   newBrkBot -= 1;
			   memcpy(newBrkBot, &tmp, sizeof(tmp));
			   // update pointers
			   scan += sizeof(tmp);
			   free += sizeof(tmp);
		       }
		       // 
		       
		       
		   } else {
		       // just move
		       memmove(free, scan, block_size);
		       // add entry to break-table (in now free space)
		       breakTableTop->brk  = (uintptr_t)scan;
		       breakTableTop->diff = scan - free;
		       breakTableTop ++;
		   }
	       } else {
		   // move block down
		   memmove(free, scan, block_size);
		   // create new break table
		   breakTableTop = (brktbl_e*) hole_end;
		   breakTableBot = breakTableTop - 1;
		   // initialize first entry
		   breakTableBot->brk  = (uintptr_t)scan;
		   breakTableBot->diff = scan - free;
		   // continue with next block
		   scan = hole_end;
	       }
	   }

	   alive += 1;
       } else {
	   // next item
	   scan += item_size;
       }
    }

    int y = 0;
    PANICF("#alive: %d\n#total: %d\n#alive_mark: %d", alive, total, marked_alive);
}

void mark(VM* vm, VAL value) {
    int i, ar;
    if(!GETGCI(value) && !ISINT(value)) {
	SETGCI(value, 1);
	marked_alive += 1;
	// propagate ALIVE
	switch(GETTY(value)) {
	case CON:
	    ar = ARITY(value);
	    for(i = 0; i < ar; ++i) {
		mark(vm, value->info.c.args[i]);
	    }
	    break;
	case STROFFSET:
	    mark(vm, value->info.str_offset->str);
	    break;
	default: // Nothing to copy
	    break;
	}
    }
}

void idris_gc(VM* vm) {
    HEAP_CHECK(vm)
    STATS_ENTER_GC(vm->stats, vm->heap.size)
    // printf("Collecting\n");
    VAL* root;
    marked_alive = 0;

    for(root = vm->valstack; root < vm->valstack_top; ++root) {
	if(*root) {
	    mark(vm, *root);
	}
    }
    if(vm->ret) mark(vm, vm->ret);
    if(vm->reg1) mark(vm, vm->reg1);

    compact(vm);
	/*
    char* newheap = malloc(vm->heap.size);
    char* oldheap = vm->heap.heap;
    if (vm->heap.old != NULL) 
        free(vm->heap.old);

    vm->heap.heap = newheap;
#ifdef FORCE_ALIGNMENT
    if (((i_int)(vm->heap.heap)&1) == 1) {
        vm->heap.next = newheap + 1;
    } else
#endif
    {
        vm->heap.next = newheap;
    }
    vm->heap.end  = newheap + vm->heap.size;

    VAL* root;

    for(root = vm->valstack; root < vm->valstack_top; ++root) {
        *root = copy(vm, *root);
    }

#ifdef HAS_PTHREAD
    for(root = vm->inbox_ptr; root < vm->inbox_write; ++root) {
        *root = copy(vm, *root);
    }
#endif
    vm->ret = copy(vm, vm->ret);
    vm->reg1 = copy(vm, vm->reg1);

    cheney(vm);

    // After reallocation, if we've still more than half filled the new heap, grow the heap
    // for next time.

    if ((vm->heap.next - vm->heap.heap) > vm->heap.size >> 1) {
        vm->heap.size += vm->heap.growth;
    } 
    vm->heap.old = oldheap;
	*/
    STATS_LEAVE_GC(vm->stats, vm->heap.size, vm->heap.next - vm->heap.heap)
    HEAP_CHECK(vm)
}

void idris_gcInfo(VM* vm, int doGC) {
    /*
    printf("Stack: <BOT %p> <TOP %p>\n", vm->valstack, vm->valstack_top); 
    printf("Final heap size         %d\n", (int)(vm->heap.size));
    printf("Final heap use          %d\n", (int)(vm->heap.next - vm->heap.heap));
    */
    if (doGC) { idris_gc(vm); }
    /*
    printf("Final heap use after GC %d\n", (int)(vm->heap.next - vm->heap.heap));

    printf("Total allocations       %d\n", vm->stats.allocations);
    printf("Number of collections   %d\n", vm->stats.collections);
    */
}
