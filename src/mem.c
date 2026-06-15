#define LOG_LVL LOG_INFO
#include "log.h"
#include "mem.h"
#include "errors.h"

static MemManager all_mem = MEM_EMPTY_MGR("GLOBAL_MEMORY_MANAGER");

typedef struct MemBlock {
    void *ptr;
    size_t size;
    const char *file;
    size_t line;
    struct MemBlock *next;
} MemBlock;

// return pointer to pointer to block with specifed addres
static MemBlock **find_mem_block(MemManager *mgr, void *ptr);
// insert block to head
static void insert_block(MemManager *mgr, MemBlock *block);

static void insert_block(MemManager *mgr, MemBlock *block) {
    logenter();
    block->next = mgr->root;
    mgr->root = block;
    mgr->current_usage += block->size + sizeof(MemBlock);
    if (mgr->current_usage > mgr->max_usage) {
        mgr->max_usage = mgr->current_usage;
    }
}

static MemBlock **find_mem_block(MemManager *mgr, void *ptr) {
    logenter();
    MemBlock **ptr_to_block_ptr = &mgr->root;
    MemBlock *block = *ptr_to_block_ptr;
    while (block) {
        if (block->ptr == ptr) {
            return ptr_to_block_ptr;
        }
        ptr_to_block_ptr = &block->next;
        block = *ptr_to_block_ptr;
    }
    return NULL;
}

void* _mem_alloc(MemManager *mgr, size_t size, const char *file, size_t line) {
    logenter();
    void *ptr = calloc(1, size);
    die(!ptr, "allocate memory: size: %zu, file: %s:%zu", size, file, line);

    MemBlock *block = (MemBlock *)malloc(sizeof(MemBlock));
    die(!block, "allocate memory for mem block: size: %zu, file: %s:%zu", size, file, line);
    MemBlock *inner_block = (MemBlock *)malloc(sizeof(MemBlock));
    die(!inner_block, "allocate memory for g mem block: size: %zu, file: %s:%zu", size, file, line);

    block->ptr = ptr;
    block->size = size;
    block->file = file;
    block->line = line;
    *inner_block = *block;

    insert_block(mgr, block);
    insert_block(&all_mem, inner_block);

    logd("Allocate %zu bytes on %p, file: %s:%zu", size, ptr, file, line);
    return ptr;
}

void* _mem_realloc(MemManager *mgr, void *ptr, size_t size, const char *file, size_t line) {
    logenter();
    MemBlock **ptr_to_block_ptr = find_mem_block(mgr, ptr);
    die(!ptr_to_block_ptr, "realloc unallocated block: addr: %p, file: %s:%zu", ptr, file, line);
    MemBlock *block = *ptr_to_block_ptr;

    MemBlock **ptr_to_inner_block_ptr = find_mem_block(&all_mem, ptr);
    die(!ptr_to_inner_block_ptr, "realloc unallocated block: addr: %p, file: %s:%zu", ptr, file, line);
    MemBlock *inner_block = *ptr_to_inner_block_ptr;

    void *new_ptr = realloc(block->ptr, size);
    die(!new_ptr, "reallocate memory for mem block: size: %zu, file: %s:%zu", size, file, line);
    logd("Realloc %zu bytes on %p, new size: %zu, on %p", block->size, block->ptr, size, new_ptr);
    inner_block->ptr = block->ptr = new_ptr;
    inner_block->size = block->size = size;
    inner_block->file = block->file = file;
    inner_block->line = block->line = line;
    return new_ptr;
}

void _mem_free(MemManager *mgr, void *ptr, const char *file, size_t line) {
    logenter();
    MemBlock **ptr_to_block_ptr = find_mem_block(mgr, ptr);
    die(!ptr_to_block_ptr, "free unallocated block: addr: %p, file: %s:%zu", ptr, file, line);
    MemBlock *block = *ptr_to_block_ptr;

    MemBlock **ptr_to_inner_block_ptr = find_mem_block(&all_mem, ptr);
    die(!ptr_to_inner_block_ptr, "free unallocated block: addr: %p, file: %s:%zu", ptr, file, line);
    MemBlock *inner_block = *ptr_to_inner_block_ptr;

    *ptr_to_block_ptr = block->next;
    *ptr_to_inner_block_ptr = inner_block->next;

    mgr->current_usage -= block->size + sizeof(MemBlock);
    all_mem.current_usage -= block->size + sizeof(MemBlock) * 2;

    logd("Free %zu bytes on %p from module: %s", block->size, block->ptr, mgr->module_name);
    free(block->ptr);
    free(block);
    free(inner_block);
}

size_t _mem_usage(struct MemManager *mgr) {
    logenter();
    return mgr->current_usage;
}

size_t _mem_max_usage(struct MemManager *mgr) {
    logenter();
    return mgr->max_usage;
}

void _mem_assert_empty(MemManager *mgr) {
    logenter();
    MemBlock *node = mgr->root;
    while (node) {
        loge("Memory leak on addr %p, size: %zu, allocated in file: %s:%zu",
              node->ptr, node->size, node->file, node->line);
        node = node->next;
    }
    die(node != mgr->root, "check all memory is free: "
        "there are still allocated blocks in module: %s", mgr->module_name);
}

void _mem_assert_all_free() {
    logenter();
    _mem_assert_empty(&all_mem);
}
