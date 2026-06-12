#ifndef MEM_H
#define MEM_H

#include <stdlib.h>

typedef struct MemBlock MemBlock;
typedef struct MemManager {
    MemBlock *root;
    size_t current_usage;
    size_t max_usage;
} MemManager;

extern MemManager mem_global_mgr;
#define MEM_GLOBAL_MGR &mem_global_mgr
#define MEM_EMPTY_MGR (MemManager){ 0 }

#define mem_alloc(mgr, size) _mem_alloc(mgr, size, __FILE__, __LINE__)
void* _mem_alloc(MemManager *mgr, size_t size, const char *file, size_t line);

#define mem_realloc(mgr, ptr, size) _mem_realloc(mgr, ptr, size, __FILE__, __LINE__)
void* _mem_realloc(MemManager *mgr, void *ptr, size_t size, const char *file, size_t line);

#define mem_free(mgr, ptr) _mem_free(mgr, ptr, __FILE__, __LINE__)
void _mem_free(MemManager *mgr, void *ptr, const char *file, size_t line);

#define mem_usage(mgr) _mem_usage(mgr)
size_t _mem_usage(MemManager *mgr);

#define mem_max_usage(mgr) _mem_max_usage(mgr)
size_t _mem_max_usage(MemManager *mgr);

#define mem_assert_all_free(mgr) _mem_assert_all_free(mgr)
void _mem_assert_all_free(MemManager *mgr);

#endif // MEM_H
