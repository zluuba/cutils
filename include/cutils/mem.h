#ifndef MEM_H
#define MEM_H

#include "cutils/location.h"

#include <stdlib.h>

typedef struct MemManager MemManager;

#define mem_mgr_new(name) _mem_mgr_new(name, location_here())
MemManager *_mem_mgr_new(const char *name, size_t limit, Location caller);
#define mem_mgr_free_all(mgr) _mem_mgr_free_all(mgr, location_here())
void _mem_mgr_free_all(MemManager *mgr, Location caller);
#define mem_mgr_del(mgr) _mem_mgr_del(mgr, location_here())
void _mem_mgr_del(MemManager *mgr, Location caller);
size_t mem_mgr_usage(MemManager *mgr);
size_t mem_mgr_max_usage(MemManager *mgr);
size_t mem_mgr_limit(MemManager *mgr);

size_t mem_global_mgr_usage(void);
size_t mem_global_mgr_max_usage(void);

#define mem_alloc(mgr, size) _mem_alloc(mgr, size, location_here())
void *_mem_alloc(MemManager *mgr, size_t size, Location caller);
#define mem_realloc(mgr, ptr, new_size) \
    _mem_realloc(mgr, ptr, new_size, location_here())
void *_mem_realloc(MemManager *mgr, void *ptr, size_t size, Location caller);
#define mem_free(mgr, ptr) _mem_free(mgr, ptr, location_here())
void _mem_free(MemManager *mgr, void *ptr, Location caller);

#endif // MEM_H
