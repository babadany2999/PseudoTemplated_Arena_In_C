#pragma once

#ifdef __cplusplus
#define RESTRICT
#else
#define RESTRICT restrict
#endif

#define unlikely(x) __builtin_expect(!!(x), 0)
#define likely(x) __builtin_expect(!!(x), 1)

#define DECLARE_DYNAMIC_ALLOCATOR_TYPE(Name, Type) \
    typedef struct DynamicAllocator##Name DynamicAllocator##Name; \

#define DECLARE_DYNAMIC_ALLOCATOR_FNS(Name, Type) \
    [[nodiscard]] bool init_dynamic_allocator_##Name(DynamicAllocator##Name*, size_t); \
    [[nodiscard]] bool realloc_dynamic_allocator_##Name(DynamicAllocator##Name*); \
    Type* alloc_dynamic_allocator_##Name(DynamicAllocator##Name*); \
    Type* push_dynamic_allocator_##Name(DynamicAllocator##Name* RESTRICT, const Type* RESTRICT); \
    Type* get_memory_dynamic_allocator_##Name(const DynamicAllocator##Name*, size_t); \
    const Type* get_memory_const_dynamic_allocator_##Name(const DynamicAllocator##Name*, size_t); \
    void reset_dynamic_allocator_##Name(DynamicAllocator##Name*); \
    void free_dynamic_allocator_##Name(DynamicAllocator##Name*);

#define DEFINE_DYNAMIC_ALLOCATOR_TYPE(Name, Type) \
    typedef struct DynamicAllocator##Name { \
        /* The memory */ \
        Type* m_memory; \
        /* How much was allocated upfront(due to INIT_DYNAMIC_ALLOCATOR) + any during REALLOC_DYNAMIC_ALLOCATOR(done internally) */ \
        size_t m_memory_count; \
        /* Cursor into the memory */ \
        size_t m_cursor; \
    } DynamicAllocator##Name;

#define DECLARE_DYNAMIC_ALLOCATOR(Name, Type) \
    DECLARE_DYNAMIC_ALLOCATOR_TYPE(Name, Type); \
    DECLARE_DYNAMIC_ALLOCATOR_FNS(Name, Type);

// API macros
#define INIT_DYNAMIC_ALLOCATOR(Name, allocator_ptr, count) \
    init_dynamic_allocator_##Name(allocator_ptr, count);

#define REALLOC_DYNAMIC_ALLOCATOR(Name, allocator_ptr) \
    realloc_dynamic_allocator_##Name(allocator_ptr);

#define ALLOC_DYNAMIC_ALLOCATOR(Name, allocator_ptr) \
    alloc_dynamic_allocator_##Name(allocator_ptr);

#define PUSH_DYNAMIC_ALLOCATOR(Name, allocator_ptr, const_type_ptr) \
    push_dynamic_allocator_##Name(allocator_ptr, const_type_ptr);

#define GET_MEMORY_DYNAMIC_ALLOCATOR(Name, allocator_ptr, index) \
    get_memory_dynamic_allocator_##Name(allocator_ptr, index);

#define GET_MEMORY_CONST_DYNAMIC_ALLOCATOR(Name, const_allocator_ptr, index) \
    get_memory_const_dynamic_allocator_##Name(const_allocator_ptr, index);

#define RESET_DYNAMIC_ALLOCATOR(Name, allocator_ptr) \
    reset_dynamic_allocator_##Name(allocator_ptr);

#define FREE_DYNAMIC_ALLOCATOR(Name, allocator_ptr) \
    free_dynamic_allocator_##Name(allocator_ptr);

#define DYNAMIC_ALLOCATOR_T(Name) \
    DynamicAllocator##Name

#ifdef DYNAMIC_ALLOCATOR_DEF
#   error "Please undef `DYNAMIC_ALLOCATOR_DEF` after you defined it"
#endif

#define DYNAMIC_ALLOCATOR_DEF(Name, Type) \
    DEFINE_DYNAMIC_ALLOCATOR_TYPE(Name, Type); \
    DECLARE_DYNAMIC_ALLOCATOR(Name, Type);
#include "dynamic_allocator.x"
#undef DYNAMIC_ALLOCATOR_DEF