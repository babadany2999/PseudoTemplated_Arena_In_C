// Splitting implementation and API macros in order to be able to
// provide definitions for custom types in testing units

// These macros are undef'ed by `dynamic_allocator.c` in order
// to allow other files(e.g., google testing TU's) to make their own
// definitions with these internals (which they should undef afterwards) 

#pragma once

#include "dynamic_allocator.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define __INTERNAL_DEFINE_IS_DYNAMIC_ALLOCATOR_VALID(Name) \
    [[nodiscard]] static bool internal_is_dynamic_allocator_valid_##Name(const DynamicAllocator##Name* allocator) { \
        return allocator != NULL && allocator->m_memory != NULL && allocator->m_memory_count > 0; \
    }

#define __INTERNAL_IS_DYNAMIC_ALLOCATOR_VALID(Name, const_allocator_ptr) \
    internal_is_dynamic_allocator_valid_##Name(const_allocator_ptr)

#define __INTERNAL_INIT_DYNAMIC_ALLOCATOR_IMPL(Name, Type) \
    bool init_dynamic_allocator_##Name(DynamicAllocator##Name* allocator, size_t count) { \
        assert(allocator != NULL); \
        assert(count > 0); \
        allocator->m_memory = (Type*)malloc(count * sizeof(Type)); \
        if (unlikely(allocator->m_memory == NULL)) { \
            return false; \
        } \
        allocator->m_memory_count = count; \
        allocator->m_cursor = 0; \
        return true; \
    }

#define __INTERNAL_REALLOC_DYNAMIC_ALLOCATOR_IMPL(Name, Type) \
    bool realloc_dynamic_allocator_##Name(DynamicAllocator##Name* allocator) { \
        assert(__INTERNAL_IS_DYNAMIC_ALLOCATOR_VALID(Name, allocator)); \
        /* Strategy is doubling the memory */ \
        const size_t new_count = 2 * allocator->m_memory_count; \
        Type* new_memory = (Type*)realloc(allocator->m_memory, new_count * sizeof(Type)); \
        if (unlikely(new_memory == NULL)) { \
            return false; \
        } \
        allocator->m_memory = new_memory; \
        allocator->m_memory_count = new_count; \
        return true; \
    }

#define __INTERNAL_ALLOC_DYNAMIC_ALLOCATOR_IMPL(Name, Type) \
    Type* alloc_dynamic_allocator_##Name(DynamicAllocator##Name* allocator) { \
        assert(__INTERNAL_IS_DYNAMIC_ALLOCATOR_VALID(Name, allocator)); \
        if (unlikely(allocator->m_cursor >= allocator->m_memory_count)) { \
            bool realloc_result = REALLOC_DYNAMIC_ALLOCATOR(Name, allocator); \
            if (unlikely(!realloc_result)) { \
                return NULL; \
            } \
        } \
        Type* ptr = &allocator->m_memory[allocator->m_cursor]; \
        ++allocator->m_cursor; \
        return ptr; \
    }

#define __INTERNAL_PUSH_DYNAMIC_ALLOCATOR_IMPL(Name, Type) \
    Type* push_dynamic_allocator_##Name(DynamicAllocator##Name* RESTRICT allocator, const Type* RESTRICT src) { \
        assert(__INTERNAL_IS_DYNAMIC_ALLOCATOR_VALID(Name, allocator)); \
        assert(src != NULL); \
        Type* new_data = ALLOC_DYNAMIC_ALLOCATOR(Name, allocator); \
        if (unlikely(new_data == NULL)) { \
            return NULL; \
        } \
        memcpy(new_data, src, sizeof(Type)); \
        return new_data; \
    }

#define __INTERNAL_GET_MEMORY_DYNAMIC_ALLOCATOR_IMPL(Name, Type) \
    Type* get_memory_dynamic_allocator_##Name(const DynamicAllocator##Name* allocator, size_t index) { \
        assert(__INTERNAL_IS_DYNAMIC_ALLOCATOR_VALID(Name, allocator)); \
        assert(index < allocator->m_cursor); \
        return &allocator->m_memory[index]; \
    }

#define __INTERNAL_GET_MEMORY_CONST_DYNAMIC_ALLOCATOR_IMPL(Name, Type) \
    const Type* get_memory_const_dynamic_allocator_##Name(const DynamicAllocator##Name* allocator, size_t index) { \
        assert(__INTERNAL_IS_DYNAMIC_ALLOCATOR_VALID(Name, allocator)); \
        assert(index < allocator->m_cursor); \
        return &allocator->m_memory[index]; \
    }

#define __INTERNAL_RESET_DYNAMIC_ALLOCATOR_IMPL(Name) \
    void reset_dynamic_allocator_##Name(DynamicAllocator##Name* allocator) { \
        assert(allocator != NULL); \
        allocator->m_cursor = 0; \
    }

#define __INTERNAL_FREE_DYNAMIC_ALLOCATOR_IMPL(Name) \
    void free_dynamic_allocator_##Name(DynamicAllocator##Name* allocator) { \
        assert(allocator != NULL); \
        assert(allocator->m_memory != NULL); \
        free(allocator->m_memory); \
        allocator->m_memory = NULL; \
        RESET_DYNAMIC_ALLOCATOR(Name, allocator); \
    }
    