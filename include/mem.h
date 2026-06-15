#ifndef MEM_H
#define MEM_H

#include <stdlib.h>

typedef struct MemBlock MemBlock;
typedef struct MemManager {
    const char *module_name;
    MemBlock *root;
    size_t current_usage;
    size_t max_usage;
} MemManager;

#define MEM_EMPTY_MGR(module_name_str) (MemManager){ .module_name = module_name_str }

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

#define mem_assert_empty(mgr) _mem_assert_empty(mgr)
void _mem_assert_empty(MemManager *mgr);

#define mem_assert_all_free() _mem_assert_all_free()
void _mem_assert_all_free(void);


#endif // MEM_H
