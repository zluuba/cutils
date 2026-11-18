#define LOG_LVL LOG_LVL_INFO
#include "log.h"
#include "mem.h"
#include "errors.h"

void* mem_safety_alloc(size_t size, Location caller) {
    log_trace("Enter");
    log_debug("Allocate ", size, " bytes");
    void *ptr = calloc(1, size);
    abort_if(ptr == NULL, "Failed to allocate ", size, " bytes.", " Caller: ",
             PriLocation(&caller));
    return ptr;
}

void* mem_safety_realloc(void *ptr, size_t size, Location caller) {
    log_trace("Enter");
    log_debug("Reallocate ptr: ", ptr, " with size: ", size, " bytes");
    void *new_ptr = realloc(ptr, size);
    abort_if(new_ptr == NULL, "Failed to reallocate ptr: ", ptr, " with size: ",
             size, " bytes.", " Caller: ", PriLocation(&caller));
    return new_ptr;
}

void mem_safety_free(void *ptr, Location caller) {
    log_trace("Enter");
    log_debug("Free ptr: ", ptr);
    abort_if(ptr == NULL, "Trying to free NULL pointer.", " Caller: ",
             PriLocation(&caller));
    free(ptr);
}

/*
typedef struct MemBlock {
    void *ptr;
    size_t size;
    Location caller;
    struct MemBlock *next;
    struct MemBlock *prev;
} MemBlock;

typedef struct MemManager {
    const char *name;
    Location caller;
    MemBlock *root;
    size_t current_usage;
    size_t max_usage;
    MemManager *next;
    MemManager *prev;
} MemManager;

struct MemManagerPool {
    MemManager *root;
} mem_mgr_pool = {};

MemManager *_mem_mgr_new(const char *name, Location caller) {
    log_trace("Enter");
    log_debug("Creating new mem mgr, caller: ", PriLocation(&caller));
    MemManager *mgr = mem_safety_alloc(sizeof(MemManager));
    mgr.caller = caller;
    mgr.name = name;
    if (mem_mgr_pool.root) {
        mem_mgr_pool.root->prev = mgr;
    }
    mgr->next = mem_mgr_pool.root;
    mem_mgr_pool.root = mgr;
    return mgr;
}

void _mem_mgr_free_all(MemManager *mgr, Location caller) {
    log_trace("Enter");
    log_debug("Deleting mem mgr, caller: ", PriLocation(&caller));
    struct MemBlock *ptr = mgr->root;
    while (ptr) {
        struct MemBlock *tmp = ptr;
        log_debug("Deleting block: ptr: ", (void *)tmp);
        ptr = ptr->next;
        mem_safety_free(tmp->ptr);
        mem_safety_free(tmp);
    }
}

void _mem_mgr_del(MemManager *mgr, Location caller) {
    log_trace("Enter");
    log_debug("Deleting mem mgr, caller: ", PriLocation(&caller));
    abort_if(mgr->root, "Attempt to delete a non-empty manager. ",
                        "Not all allocated memory has been released. ",
                        "Caller: ", PriLocation(&caller));
    if (mgr->prev) {
        mgr->prev->next = mgr->next;
    }
    if (mgr->next) {
        mgr->next->prev = mgr->prev;
    }
    mem_safety_free(mgr);
}

size_t mem_mgr_usage(MemManager *mgr) {
    log_trace("Enter");
    return mgr->current_usage;
}

size_t mem_mgr_max_usage(MemManager *mgr) {
    log_trace("Enter");
    return mgr->max_usage;
}

size_t mem_mgr_limit(MemManager *mgr) {
    log_trace("Enter");
    return mgr->limit;
}

size_t mem_global_mgr_usage(void);
size_t mem_global_mgr_max_usage(void);

void *_mem_alloc(MemManager *mgr, size_t size, Location caller) {
    log_trace("Enter");
    log_debug("Allocate ", size, " bytes. Manager: ", mgr->name);
    void mem_ptr = mem_safety_alloc(size, caller);
    MemBlock *block = mem_safety_alloc(sizeof(MemBlock), caller);
    block->caller = caller;
    block->ptr = mem_ptr;
    block->size = size;
    if (mgr->root) {
        mgr->root->prev = block;
    }
    block->next = mgr->root;
    mgr->root = block;
    mgr->current_usage += size;
    if (mgr->max_usage < mgr->current_usage) {
        mgr->max_usage = mgr->current_usage;
    }
    return mem_ptr;
}

MemBlock *get_block_by_ptr(MemManager *mgr, void *ptr) {
    MemBlock *block = mgr->root;
    while (block && block->ptr != ptr) {
        block = block->next;
    }
    return block;
}

void *_mem_realloc(MemManager *mgr, void *ptr, size_t size, Location caller) {
    log_trace("Enter");
    log_debug("Reallocate ", PriPtr(ptr), " , new size: ", size, ". ",
              "Manager: ", mgr->name);
    MemBlock *block = get_block_by_ptr(mgr, ptr);
    abort_if(!block, "The specified pointer does not belong to this manager. ",
                     "Manager: ", mgr->name, ", pointer: ", ptr, ", "
                     "caller: ", PriLocation(&caller));
    mgr->current_usage -= block->size;
    block->ptr = mem_safety_realloc(block->ptr, size, caller);
    block->size = size;
    mgr->current_usage += block->size;
    if (mgr->max_usage < mgr->current_usage) {
        mgr->max_usage = mgr->current_usage;
    }
    return block->ptr;
}

void _mem_free(MemManager *mgr, void *ptr, Location caller) {
    log_trace("Enter");
    log_debug("Free ", PriPtr(ptr), ". Manager: ", mgr->name);
    MemBlock *block = get_block_by_ptr(mgr, ptr);
    abort_if(!block, "The specified pointer does not belong to this manager. "
                     "Manager: ", mgr->name, ", pointer: ", ptr, ", "
                     "caller: ", PriLocation(&caller));
    if (block->prev) {
        block->prev->next = block->next;
    }
    if (block->next) {
        block->next->prev = block->prev;
    }
    mem_safety_free(block->ptr, caller);
    mem_safety_free(block, caller);
}








// static MemManager all_mem = MEM_EMPTY_MGR("GLOBAL_MEMORY_MANAGER");

// typedef struct MemBlock {
//     void *ptr;
//     size_t size;
//     const char *file;
//     size_t line;
//     struct MemBlock *next;
// } MemBlock;

// // return pointer to pointer to block with specifed addres
// static MemBlock **find_mem_block(MemManager *mgr, void *ptr);
// // insert block to head
// static void insert_block(MemManager *mgr, MemBlock *block);

// static void insert_block(MemManager *mgr, MemBlock *block) {
//     logenter();
//     block->next = mgr->root;
//     mgr->root = block;
//     mgr->current_usage += block->size + sizeof(MemBlock);
//     if (mgr->current_usage > mgr->max_usage) {
//         mgr->max_usage = mgr->current_usage;
//     }
// }

// static MemBlock **find_mem_block(MemManager *mgr, void *ptr) {
//     logenter();
//     MemBlock **ptr_to_block_ptr = &mgr->root;
//     MemBlock *block = *ptr_to_block_ptr;
//     while (block) {
//         if (block->ptr == ptr) {
//             return ptr_to_block_ptr;
//         }
//         ptr_to_block_ptr = &block->next;
//         block = *ptr_to_block_ptr;
//     }
//     return NULL;
// }

// void* _mem_alloc(MemManager *mgr, size_t size, const char *file, size_t line) {
//     logenter();
//     void *ptr = calloc(1, size);
//     die(!ptr, "allocate memory: size: %zu, file: %s:%zu", size, file, line);

//     MemBlock *block = (MemBlock *)malloc(sizeof(MemBlock));
//     die(!block, "allocate memory for mem block: size: %zu, file: %s:%zu", size, file, line);
//     MemBlock *inner_block = (MemBlock *)malloc(sizeof(MemBlock));
//     die(!inner_block, "allocate memory for g mem block: size: %zu, file: %s:%zu", size, file, line);

//     block->ptr = ptr;
//     block->size = size;
//     block->file = file;
//     block->line = line;
//     *inner_block = *block;

//     insert_block(mgr, block);
//     insert_block(&all_mem, inner_block);

//     logd("Allocate %zu bytes on %p, file: %s:%zu", size, ptr, file, line);
//     return ptr;
// }

// void* _mem_realloc(MemManager *mgr, void *ptr, size_t size, const char *file, size_t line) {
//     logenter();
//     MemBlock **ptr_to_block_ptr = find_mem_block(mgr, ptr);
//     die(!ptr_to_block_ptr, "realloc unallocated block: addr: %p, file: %s:%zu", ptr, file, line);
//     MemBlock *block = *ptr_to_block_ptr;

//     MemBlock **ptr_to_inner_block_ptr = find_mem_block(&all_mem, ptr);
//     die(!ptr_to_inner_block_ptr, "realloc unallocated block: addr: %p, file: %s:%zu", ptr, file, line);
//     MemBlock *inner_block = *ptr_to_inner_block_ptr;

//     void *new_ptr = realloc(block->ptr, size);
//     die(!new_ptr, "reallocate memory for mem block: size: %zu, file: %s:%zu", size, file, line);
//     logd("Realloc %zu bytes on %p, new size: %zu, on %p", block->size, block->ptr, size, new_ptr);
//     inner_block->ptr = block->ptr = new_ptr;
//     inner_block->size = block->size = size;
//     inner_block->file = block->file = file;
//     inner_block->line = block->line = line;
//     return new_ptr;
// }

// void _mem_free(MemManager *mgr, void *ptr, const char *file, size_t line) {
//     logenter();
//     MemBlock **ptr_to_block_ptr = find_mem_block(mgr, ptr);
//     die(!ptr_to_block_ptr, "free unallocated block: addr: %p, file: %s:%zu", ptr, file, line);
//     MemBlock *block = *ptr_to_block_ptr;

//     MemBlock **ptr_to_inner_block_ptr = find_mem_block(&all_mem, ptr);
//     die(!ptr_to_inner_block_ptr, "free unallocated block: addr: %p, file: %s:%zu", ptr, file, line);
//     MemBlock *inner_block = *ptr_to_inner_block_ptr;

//     *ptr_to_block_ptr = block->next;
//     *ptr_to_inner_block_ptr = inner_block->next;

//     mgr->current_usage -= block->size + sizeof(MemBlock);
//     all_mem.current_usage -= block->size + sizeof(MemBlock) * 2;

//     logd("Free %zu bytes on %p from module: %s", block->size, block->ptr, mgr->module_name);
//     free(block->ptr);
//     free(block);
//     free(inner_block);
// }

// size_t _mem_usage(struct MemManager *mgr) {
//     logenter();
//     return mgr->current_usage;
// }

// size_t _mem_max_usage(struct MemManager *mgr) {
//     logenter();
//     return mgr->max_usage;
// }

// void _mem_assert_empty(MemManager *mgr) {
//     logenter();
//     MemBlock *node = mgr->root;
//     while (node) {
//         loge("Memory leak on addr %p, size: %zu, allocated in file: %s:%zu",
//               node->ptr, node->size, node->file, node->line);
//         node = node->next;
//     }
//     die(node != mgr->root, "check all memory is free: "
//         "there are still allocated blocks in module: %s", mgr->module_name);
// }

// void _mem_assert_all_free(void) {
//     logenter();
//     _mem_assert_empty(&all_mem);
// }

// */
