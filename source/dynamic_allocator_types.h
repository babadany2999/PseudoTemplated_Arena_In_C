#pragma once

#define DECLARE_DYNAMIC_ALLOCATOR_TYPE(Name, Type) \
    typedef struct DynamicAllocator##Name DynamicAllocator##Name; \

#ifdef DYNAMIC_ALLOCATOR_DEF
#   error "Please undef `DYNAMIC_ALLOCATOR_DEF` after you defined it"
#endif

#define DYNAMIC_ALLOCATOR_DEF(Name, Type) \
    DECLARE_DYNAMIC_ALLOCATOR_TYPE(Name, Type);
#include "implementation/utils/dynamic_allocator.x"
#undef DYNAMIC_ALLOCATOR_DEF