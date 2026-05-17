#include <gtest/gtest.h>

#include <type_traits>
#include <utility>

extern "C" {
    #include "dynamic_allocator_internal.h"
}

// The types to be tested
struct __Test_PlayerData {
    uint64_t id;
    uint32_t health;
    uint32_t mana;
};

struct __Test_Vector3 {
    float x;
    float y;
    float z;
};

#ifdef __TEST_DYNAMIC_ALLOCATOR_DEF
#   error "Please undef `__TEST_DYNAMIC_ALLOCATOR_DEF` after you defined it"
#endif

#define __TEST_DEFINE_INIT_DYNAMIC_ALLOCATOR(Name, Type) \
    __INTERNAL_INIT_DYNAMIC_ALLOCATOR_IMPL(Name, Type);

#define __TEST_DEFINE_REALLOC_DYNAMIC_ALLOCATOR(Name, Type) \
    __INTERNAL_REALLOC_DYNAMIC_ALLOCATOR_IMPL(Name, Type);

#define __TEST_DEFINE_ALLOC_DYNAMIC_ALLOCATOR(Name, Type) \
    __INTERNAL_ALLOC_DYNAMIC_ALLOCATOR_IMPL(Name, Type);

#define __TEST_DEFINE_PUSH_DYNAMIC_ALLOCATOR(Name, Type) \
    __INTERNAL_PUSH_DYNAMIC_ALLOCATOR_IMPL(Name, Type);

#define __TEST_DEFINE_GET_MEMORY_DYNAMIC_ALLOCATOR(Name, Type) \
    __INTERNAL_GET_MEMORY_DYNAMIC_ALLOCATOR_IMPL(Name, Type);

#define __TEST_DEFINE_GET_MEMORY_CONST_DYNAMIC_ALLOCATOR(Name, Type) \
    __INTERNAL_GET_MEMORY_CONST_DYNAMIC_ALLOCATOR_IMPL(Name, Type);

#define __TEST_DEFINE_RESET_DYNAMIC_ALLOCATOR(Name) \
    __INTERNAL_RESET_DYNAMIC_ALLOCATOR_IMPL(Name);

#define __TEST_FREE_DYNAMIC_ALLOCATOR(Name) \
    __INTERNAL_FREE_DYNAMIC_ALLOCATOR_IMPL(Name);

#define __TEST_DYNAMIC_ALLOCATOR_DEF(Name, Type, NameOfArrayContainingTestValues) \
DEFINE_DYNAMIC_ALLOCATOR_TYPE(Name, Type); \
namespace { \
    DECLARE_DYNAMIC_ALLOCATOR(Name, Type); \
    __INTERNAL_DEFINE_IS_DYNAMIC_ALLOCATOR_VALID(Name); \
    __INTERNAL_INIT_DYNAMIC_ALLOCATOR_IMPL(Name, Type); \
    __INTERNAL_REALLOC_DYNAMIC_ALLOCATOR_IMPL(Name, Type); \
    __INTERNAL_ALLOC_DYNAMIC_ALLOCATOR_IMPL(Name, Type); \
    __INTERNAL_PUSH_DYNAMIC_ALLOCATOR_IMPL(Name, Type); \
    __INTERNAL_GET_MEMORY_DYNAMIC_ALLOCATOR_IMPL(Name, Type); \
    __INTERNAL_GET_MEMORY_CONST_DYNAMIC_ALLOCATOR_IMPL(Name, Type); \
    __INTERNAL_RESET_DYNAMIC_ALLOCATOR_IMPL(Name); \
    __INTERNAL_FREE_DYNAMIC_ALLOCATOR_IMPL(Name); \
} /* namespace */

#include "dynamic_allocator.test.xpp"
#undef __TEST_DYNAMIC_ALLOCATOR_DEF

#   undef DECLARE_DYNAMIC_ALLOCATOR
#   undef DEFINE_DYNAMIC_ALLOCATOR_TYPE
#   undef __INTERNAL_DEFINE_IS_DYNAMIC_ALLOCATOR_VALID
#   undef __INTERNAL_INIT_DYNAMIC_ALLOCATOR_IMPL
#   undef __INTERNAL_REALLOC_DYNAMIC_ALLOCATOR_IMPL
#   undef __INTERNAL_ALLOC_DYNAMIC_ALLOCATOR_IMPL
#   undef __INTERNAL_PUSH_DYNAMIC_ALLOCATOR_IMPL
#   undef __INTERNAL_GET_MEMORY_DYNAMIC_ALLOCATOR_IMPL
#   undef __INTERNAL_GET_MEMORY_CONST_DYNAMIC_ALLOCATOR_IMPL
#   undef __INTERNAL_RESET_DYNAMIC_ALLOCATOR_IMPL
#   undef __INTERNAL_FREE_DYNAMIC_ALLOCATOR_IMPL

namespace {

template <typename TDynamicAllocator>
using init_dynamic_allocator_fn_signature = bool(*)(TDynamicAllocator*, size_t);

template <typename TDynamicAllocator>
using realloc_dynamic_allocator_fn_signature = bool(*)(TDynamicAllocator*);

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
using alloc_dynamic_allocator_fn_signature = TDynamicAllocatorDataType*(*)(TDynamicAllocator*);

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
using push_dynamic_allocator_fn_signature = TDynamicAllocatorDataType*(*)(TDynamicAllocator*, const TDynamicAllocatorDataType*);

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
using get_memory_dynamic_allocator_fn_signature = TDynamicAllocatorDataType*(*)(TDynamicAllocator*, size_t);

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
using get_memory_const_dynamic_allocator_fn_signature = const TDynamicAllocatorDataType*(*)(const TDynamicAllocator*, size_t);

template <typename TDynamicAllocator>
using reset_dynamic_allocator_fn_signature = void(*)(TDynamicAllocator*);

template <typename TDynamicAllocator>
using free_dynamic_allocator_fn_signature = void(*)(TDynamicAllocator*);

} // namespace

#define __TEST_MAKE_INIT_DYNAMIC_ALLOCATOR_FN_PTR(Name) \
    [](DYNAMIC_ALLOCATOR_T(Name)* allocator, size_t count) -> bool { return INIT_DYNAMIC_ALLOCATOR(Name, allocator, count); };

#define __TEST_MAKE_REALLOC_DYNAMIC_ALLOCATOR_FN_PTR(Name) \
    [](DYNAMIC_ALLOCATOR_T(Name)* allocator) -> bool { return REALLOC_DYNAMIC_ALLOCATOR(Name, allocator); };

#define __TEST_MAKE_ALLOC_DYNAMIC_ALLOCATOR_FN_PTR(Name, Type) \
    [](DYNAMIC_ALLOCATOR_T(Name)* allocator) -> Type* { return ALLOC_DYNAMIC_ALLOCATOR(Name, allocator); };

#define __TEST_MAKE_PUSH_DYNAMIC_ALLOCATOR_FN_PTR(Name, Type) \
    [](DYNAMIC_ALLOCATOR_T(Name)* RESTRICT allocator, const Type* RESTRICT src) -> Type* { return PUSH_DYNAMIC_ALLOCATOR(Name, allocator, src); };

#define __TEST_MAKE_GET_MEMORY_DYNAMIC_ALLOCATOR_FN_PTR(Name, Type) \
    [](DYNAMIC_ALLOCATOR_T(Name)* allocator, size_t index) -> Type* { return GET_MEMORY_DYNAMIC_ALLOCATOR(Name, allocator, index); };

#define __TEST_MAKE_GET_MEMORY_CONST_DYNAMIC_ALLOCATOR_FN_PTR(Name, Type) \
    [](const DYNAMIC_ALLOCATOR_T(Name)* allocator, size_t index) -> const Type* { return GET_MEMORY_CONST_DYNAMIC_ALLOCATOR(Name, allocator, index); };

#define __TEST_MAKE_RESET_DYNAMIC_ALLOCATOR_FN_PTR(Name) \
    [](DYNAMIC_ALLOCATOR_T(Name)* allocator) -> void { RESET_DYNAMIC_ALLOCATOR(Name, allocator); };

#define __TEST_MAKE_FREE_DYNAMIC_ALLOCATOR_FN_PTR(Name) \
    [](DYNAMIC_ALLOCATOR_T(Name)* allocator) -> void { FREE_DYNAMIC_ALLOCATOR(Name, allocator); };

#define __TEST_MAKE_FN_PTRS(Name, Type) \
    auto init_fn_ptr = __TEST_MAKE_INIT_DYNAMIC_ALLOCATOR_FN_PTR(Name); \
    auto realloc_fn_ptr = __TEST_MAKE_REALLOC_DYNAMIC_ALLOCATOR_FN_PTR(Name); \
    auto alloc_fn_ptr = __TEST_MAKE_ALLOC_DYNAMIC_ALLOCATOR_FN_PTR(Name, Type); \
    auto push_fn_ptr = __TEST_MAKE_PUSH_DYNAMIC_ALLOCATOR_FN_PTR(Name, Type); \
    auto get_memory_fn_ptr = __TEST_MAKE_GET_MEMORY_DYNAMIC_ALLOCATOR_FN_PTR(Name, Type); \
    auto get_memory_const_fn_ptr = __TEST_MAKE_GET_MEMORY_CONST_DYNAMIC_ALLOCATOR_FN_PTR(Name, Type); \
    auto reset_fn_ptr = __TEST_MAKE_RESET_DYNAMIC_ALLOCATOR_FN_PTR(Name); \
    auto free_fn_ptr = __TEST_MAKE_FREE_DYNAMIC_ALLOCATOR_FN_PTR(Name);

// Fixture to create a DynamicAllocator and free it automatically
template <typename T>
class DynamicAllocatorBaseFixture : public ::testing::Test {
protected:
    T allocator{};
    // Nice size for initial allocations made, could make another
    // fixture to stress-test it with a bigger initial allocation size
    static constexpr size_t count = 10;
};

#define __TEST_MAKE_ISOLATED_TEST(TestScope, TestName, TestContent) \
    TEST(TestScope, TestName) { \
        (TestContent); \
    }

#define __TEST_MAKE_FIXTURE_TEST(TestScope, TestName, TestFn, ...) \
    TEST_F(TestScope, TestName) { \
        TestFn(allocator, ##__VA_ARGS__); \
    }

namespace {

template <typename TDynamicAllocator>
void __test_dynamic_allocator_init_death_if_invalid_input(init_dynamic_allocator_fn_signature<TDynamicAllocator> init_fn_ptr) {
    TDynamicAllocator allocator{};

    const size_t count = 2;

    EXPECT_DEATH((void)init_fn_ptr(nullptr, static_cast<size_t>(0)), "");
    EXPECT_DEATH((void)init_fn_ptr(&allocator, static_cast<size_t>(0)), "");
    EXPECT_DEATH((void)init_fn_ptr(nullptr, count), "");
}

template <typename TDynamicAllocator>
void __test_dynamic_allocator_init_valid_init(init_dynamic_allocator_fn_signature<TDynamicAllocator> init_fn_ptr) {
    TDynamicAllocator allocator{};
    const size_t count = 2;
    
    bool r = init_fn_ptr(&allocator, count);
    EXPECT_TRUE(r);
    EXPECT_NE(allocator.m_memory, nullptr);
    EXPECT_GT(allocator.m_memory_count, static_cast<size_t>(0));
    EXPECT_EQ(allocator.m_cursor, static_cast<size_t>(0));
}

} // namespace

#define __TEST_DYNAMIC_ALLOCATOR_INIT(Name) \
    __TEST_MAKE_ISOLATED_TEST( \
        DynamicAllocatorInit##Name, \
        DeathIfInvalidInput, \
        (__test_dynamic_allocator_init_death_if_invalid_input<DYNAMIC_ALLOCATOR_T(Name)>(init_fn_ptr)) \
    ); \
    __TEST_MAKE_ISOLATED_TEST( \
        DynamicAllocatorInit##Name, \
        ValidInit, \
        (__test_dynamic_allocator_init_valid_init<DYNAMIC_ALLOCATOR_T(Name)>(init_fn_ptr)) \
    );

namespace {

template <typename TDynamicAllocator>
void __test_dynamic_allocator_realloc_death_if_invalid_input(
    init_dynamic_allocator_fn_signature<TDynamicAllocator> init_fn_ptr,
    realloc_dynamic_allocator_fn_signature<TDynamicAllocator> realloc_fn_ptr,
    free_dynamic_allocator_fn_signature<TDynamicAllocator> free_fn_ptr
) {
    EXPECT_DEATH((void)realloc_fn_ptr(nullptr), "");

    TDynamicAllocator allocator{};
    const size_t count = 2;

    (void)init_fn_ptr(&allocator, count);
    
    auto* memory_ptr = allocator.m_memory;
    allocator.m_memory = nullptr;
    EXPECT_DEATH((void)realloc_fn_ptr(&allocator), "");
    allocator.m_memory = memory_ptr;
    
    auto memory_count = allocator.m_memory_count;
    allocator.m_memory_count = 0;
    EXPECT_DEATH((void)realloc_fn_ptr(&allocator), "");
    allocator.m_memory_count = memory_count;

    (void)free_fn_ptr(&allocator);
}

template <typename TDynamicAllocator>
void __test_dynamic_allocator_realloc_valid_realloc(
    init_dynamic_allocator_fn_signature<TDynamicAllocator> init_fn_ptr,
    realloc_dynamic_allocator_fn_signature<TDynamicAllocator> realloc_fn_ptr,
    free_dynamic_allocator_fn_signature<TDynamicAllocator> free_fn_ptr
) {
    TDynamicAllocator allocator{};
    const size_t count = 2;
    (void)init_fn_ptr(&allocator, count);

    const size_t old_memory_count = allocator.m_memory_count;

    bool r = realloc_fn_ptr(&allocator);
    EXPECT_TRUE(r);
    EXPECT_GT(allocator.m_memory_count, old_memory_count);

    (void)free_fn_ptr(&allocator);
}

} // namespace

#define __TEST_DYNAMIC_ALLOCATOR_REALLOC(Name) \
    __TEST_MAKE_ISOLATED_TEST( \
        DynamicAllocatorRealloc##Name, \
        DeathIfInvalidInput, \
        (__test_dynamic_allocator_realloc_death_if_invalid_input<DYNAMIC_ALLOCATOR_T(Name)>(init_fn_ptr, realloc_fn_ptr, free_fn_ptr)) \
    ); \
    __TEST_MAKE_ISOLATED_TEST( \
        DynamicAllocatorRealloc##Name, \
        ValidRealloc, \
        (__test_dynamic_allocator_realloc_valid_realloc<DYNAMIC_ALLOCATOR_T(Name)>(init_fn_ptr, realloc_fn_ptr, free_fn_ptr)) \
    );

namespace {

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
void __test_dynamic_allocator_alloc_death_if_invalid_input(
    init_dynamic_allocator_fn_signature<TDynamicAllocator> init_fn_ptr,
    alloc_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> alloc_fn_ptr,
    free_dynamic_allocator_fn_signature<TDynamicAllocator> free_fn_ptr
) {
    EXPECT_DEATH((void)alloc_fn_ptr(nullptr), "");

    TDynamicAllocator allocator{};
    const size_t count = 2;

    (void)init_fn_ptr(&allocator, count);
    
    auto* memory_ptr = allocator.m_memory;
    allocator.m_memory = nullptr;
    EXPECT_DEATH((void)alloc_fn_ptr(&allocator), "");
    allocator.m_memory = memory_ptr;
    
    auto memory_count = allocator.m_memory_count;
    allocator.m_memory_count = 0;
    EXPECT_DEATH((void)alloc_fn_ptr(&allocator), "");
    allocator.m_memory_count = memory_count;

    (void)free_fn_ptr(&allocator);
}

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
void __test_dynamic_allocator_alloc_realloc_triggered_if_cursor_past_memory_count(
    TDynamicAllocator& allocator,
    alloc_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> alloc_fn_ptr
) 
{
    const size_t old_cursor = allocator.m_cursor;
    const size_t old_memory_count = allocator.m_memory_count;

    allocator.m_cursor = old_memory_count + 1;

    alloc_fn_ptr(&allocator);
    EXPECT_GT(allocator.m_memory_count, old_memory_count);

    // Reset state
    allocator.m_cursor = old_cursor;
}

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
void __test_dynamic_allocator_alloc_valid_alloc(
    TDynamicAllocator& allocator,
    alloc_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> alloc_fn_ptr
) 
{
    const size_t old_cursor = allocator.m_cursor;

    auto* alloc_ptr = alloc_fn_ptr(&allocator);
    ASSERT_NE(alloc_ptr, nullptr);
    EXPECT_EQ(alloc_ptr, &allocator.m_memory[old_cursor]);
    EXPECT_EQ(allocator.m_cursor, old_cursor + 1);
}

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
void __test_dynamic_allocator_alloc_valid_alloc_even_after_realloc_was_triggered(
    TDynamicAllocator& allocator,
    alloc_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> alloc_fn_ptr
)
{
    const size_t old_memory_count = allocator.m_memory_count;

    // Allocate just enough to fill the current count
    for (size_t index = 0; index < allocator.m_memory_count; ++index) {
        alloc_fn_ptr(&allocator);
    }

    ASSERT_EQ(allocator.m_memory_count, old_memory_count); // Shouldn't have changed
    ASSERT_EQ(allocator.m_cursor, allocator.m_memory_count);

    // Final allocation to be just past `m_memory_count`
    auto* alloc_ptr = alloc_fn_ptr(&allocator);
    ASSERT_NE(alloc_ptr, nullptr);
    EXPECT_GT(allocator.m_memory_count, old_memory_count);
    EXPECT_EQ(allocator.m_cursor, old_memory_count + 1);
}

} // namespace

#define __TEST_DYNAMIC_ALLOCATOR_ALLOC(Name, Type) \
    __TEST_MAKE_ISOLATED_TEST( \
        DynamicAllocatorAlloc##Name, \
        DeathIfInvalidInput, \
        (__test_dynamic_allocator_alloc_death_if_invalid_input<DYNAMIC_ALLOCATOR_T(Name), Type>(init_fn_ptr, alloc_fn_ptr, free_fn_ptr)) \
    ); \
    __TEST_MAKE_FIXTURE_TEST( \
        DynamicAllocatorAllocFixture##Name, \
        ReallocTriggeredIfCursorPastMemoryCount, \
        (__test_dynamic_allocator_alloc_realloc_triggered_if_cursor_past_memory_count<DYNAMIC_ALLOCATOR_T(Name), Type>), \
        alloc_fn_ptr \
    ); \
    __TEST_MAKE_FIXTURE_TEST( \
        DynamicAllocatorAllocFixture##Name, \
        ValidAlloc, \
        (__test_dynamic_allocator_alloc_valid_alloc<DYNAMIC_ALLOCATOR_T(Name), Type>), \
        alloc_fn_ptr \
    ); \
    __TEST_MAKE_FIXTURE_TEST( \
        DynamicAllocatorAllocFixture##Name, \
        ValidAllocEvenAfterReallocWasTriggered, \
        (__test_dynamic_allocator_alloc_valid_alloc_even_after_realloc_was_triggered<DYNAMIC_ALLOCATOR_T(Name), Type>), \
        alloc_fn_ptr \
    );

namespace {

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
void __test_dynamic_allocator_push_death_if_invalid_input(
    TDynamicAllocator& allocator,
    push_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> push_fn_ptr
) 
{
    auto* memory = allocator.m_memory;
    EXPECT_DEATH(push_fn_ptr(nullptr, nullptr), "");
    EXPECT_DEATH(push_fn_ptr(nullptr, memory), "");
    EXPECT_DEATH(push_fn_ptr(&allocator, nullptr), "");
}

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
void __test_dynamic_allocator_push_pushed(
    TDynamicAllocator& allocator,
    const TDynamicAllocatorDataType& test_value,
    push_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> push_fn_ptr
)
{
    auto* new_push_ptr = push_fn_ptr(&allocator, &test_value);
    EXPECT_NE(new_push_ptr, nullptr);
    EXPECT_EQ(allocator.m_cursor, static_cast<size_t>(1));
    EXPECT_EQ(&allocator.m_memory[allocator.m_cursor - 1], new_push_ptr);
    EXPECT_EQ(memcmp(new_push_ptr, &test_value, sizeof(TDynamicAllocatorDataType)), 0);
}

} // namespace

#define __TEST_DYNAMIC_ALLOCATOR_PUSH(Name, Type, test_memory) \
    __TEST_MAKE_FIXTURE_TEST( \
        DynamicAllocatorPushFixture##Name, \
        DeathIfInvalidInput, \
        (__test_dynamic_allocator_push_death_if_invalid_input<DYNAMIC_ALLOCATOR_T(Name), Type>), \
        push_fn_ptr \
    ); \
    __TEST_MAKE_FIXTURE_TEST( \
        DynamicAllocatorPushFixture##Name, \
        Pushed, \
        (__test_dynamic_allocator_push_pushed<DYNAMIC_ALLOCATOR_T(Name), Type>), \
        test_memory, \
        push_fn_ptr \
    );

namespace {

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType, typename TGetFnPtr>
constexpr bool is_valid_get_fn_ptr_v =
    std::is_same_v<TGetFnPtr, get_memory_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType>> ||
    std::is_same_v<TGetFnPtr, get_memory_const_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType>>;

// Same tests are needed for both const and non-const getters
template <
    typename TDynamicAllocator, 
    typename TDynamicAllocatorDataType, 
    typename TGetFnPtr,
    typename = std::enable_if_t<
        is_valid_get_fn_ptr_v<
            TDynamicAllocator,
            TDynamicAllocatorDataType,
            TGetFnPtr
        >, 
        void
    >
>
void __test_dynamic_allocator_getters_memory_death_if_invalid_input_impl(
    TDynamicAllocator& allocator,
    TGetFnPtr get_memory_fn_ptr
)
{
    EXPECT_DEATH(get_memory_fn_ptr(nullptr, 0), "");
    EXPECT_DEATH(get_memory_fn_ptr(&allocator, 2), "");
}

template <
    typename TDynamicAllocator, 
    typename TDynamicAllocatorDataType, 
    typename TGetFnPtr,
    typename = std::enable_if_t<
        is_valid_get_fn_ptr_v<
            TDynamicAllocator,
            TDynamicAllocatorDataType,
            TGetFnPtr
        >, 
        void
    >
>
void __test_dynamic_allocator_getters_memory_returns_proper_memory_impl(
    TDynamicAllocator& allocator,
    const TDynamicAllocatorDataType& test_value_one,
    const TDynamicAllocatorDataType& test_value_two,
    push_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> push_fn_ptr,
    TGetFnPtr get_memory_fn_ptr
)
{
    push_fn_ptr(&allocator, &test_value_one);

    auto* get_ptr = get_memory_fn_ptr(&allocator, 0);
    EXPECT_NE(get_ptr, nullptr);
    EXPECT_EQ(memcmp(get_ptr, &test_value_one, sizeof(TDynamicAllocatorDataType)), 0);

    push_fn_ptr(&allocator, &test_value_one);
    auto* get_ptr_2 = get_memory_fn_ptr(&allocator, 1);
    EXPECT_NE(get_ptr_2, nullptr);
    EXPECT_NE(get_ptr_2, get_ptr);
    EXPECT_EQ(memcmp(get_ptr_2, &test_value_one, sizeof(TDynamicAllocatorDataType)), 0);

    push_fn_ptr(&allocator, &test_value_two);

    auto* get_ptr_3 = get_memory_fn_ptr(&allocator, 2);
    EXPECT_NE(get_ptr_3, nullptr);
    EXPECT_NE(get_ptr_3, get_ptr_2);
    EXPECT_EQ(memcmp(get_ptr_3, &test_value_two, sizeof(TDynamicAllocatorDataType)), 0);
}

template <
    typename TDynamicAllocator, 
    typename TDynamicAllocatorDataType, 
    typename TGetFnPtr,
    typename = std::enable_if_t<
        is_valid_get_fn_ptr_v<
            TDynamicAllocator,
            TDynamicAllocatorDataType,
            TGetFnPtr
        >, 
        void
    >
>
void __test_dynamic_allocator_getters_memory_death_after_reset_with_index_past_cursor_impl(
    TDynamicAllocator& allocator,
    const TDynamicAllocatorDataType& test_value_one,
    push_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> push_fn_ptr,
    TGetFnPtr get_memory_fn_ptr,
    reset_dynamic_allocator_fn_signature<TDynamicAllocator> reset_fn_ptr
)
{
    push_fn_ptr(&allocator, &test_value_one);
    reset_fn_ptr(&allocator);

    EXPECT_DEATH(get_memory_fn_ptr(&allocator, 0), "");
}

} // namespace

namespace {

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
void __test_dynamic_allocator_get_memory_death_if_invalid_input(
    TDynamicAllocator& allocator,
    get_memory_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> get_memory_fn_ptr
)
{
    __test_dynamic_allocator_getters_memory_death_if_invalid_input_impl<
        TDynamicAllocator, 
        TDynamicAllocatorDataType, 
        decltype(get_memory_fn_ptr)
    >(allocator, get_memory_fn_ptr);
}

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
void __test_dynamic_allocator_get_memory_returns_proper_memory(
    TDynamicAllocator& allocator,
    const TDynamicAllocatorDataType& test_value_one,
    const TDynamicAllocatorDataType& test_value_two,
    push_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> push_fn_ptr,
    get_memory_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> get_memory_fn_ptr
)
{
    __test_dynamic_allocator_getters_memory_returns_proper_memory_impl<
        TDynamicAllocator, 
        TDynamicAllocatorDataType, 
        decltype(get_memory_fn_ptr)
    >(allocator, test_value_one, test_value_two, push_fn_ptr, get_memory_fn_ptr);
}

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
void __test_dynamic_allocator_get_memory_death_after_reset_with_index_past_cursor(
    TDynamicAllocator& allocator,
    const TDynamicAllocatorDataType& test_value_one,
    push_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> push_fn_ptr,
    get_memory_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> get_memory_fn_ptr,
    reset_dynamic_allocator_fn_signature<TDynamicAllocator> reset_fn_ptr
)
{
    __test_dynamic_allocator_getters_memory_death_after_reset_with_index_past_cursor_impl<
        TDynamicAllocator,
        TDynamicAllocatorDataType,
        decltype(get_memory_fn_ptr)
    >
    (
        allocator, 
        test_value_one,
        push_fn_ptr, 
        get_memory_fn_ptr, 
        reset_fn_ptr
    );
}

} // namespace

#define __TEST_DYNAMIC_ALLOCATOR_GET_MEMORY(Name, Type, test_value_one, test_value_two) \
    __TEST_MAKE_FIXTURE_TEST( \
        DynamicAllocatorGetMemoryFixture##Name, \
        DeathIfInvalidInput, \
        (__test_dynamic_allocator_get_memory_death_if_invalid_input<DYNAMIC_ALLOCATOR_T(Name), Type>), \
        get_memory_fn_ptr \
    ); \
    __TEST_MAKE_FIXTURE_TEST( \
        DynamicAllocatorGetMemoryFixture##Name, \
        ReturnsProperMemory, \
        (__test_dynamic_allocator_get_memory_returns_proper_memory<DYNAMIC_ALLOCATOR_T(Name), Type>), \
        test_value_one, \
        test_value_two, \
        push_fn_ptr, \
        get_memory_fn_ptr \
    ); \
    __TEST_MAKE_FIXTURE_TEST( \
        DynamicAllocatorGetMemoryFixture##Name, \
        DeathAfterResetWithIndexPastCursor, \
        (__test_dynamic_allocator_get_memory_death_after_reset_with_index_past_cursor<DYNAMIC_ALLOCATOR_T(Name), Type>), \
        test_value_one, \
        push_fn_ptr, \
        get_memory_fn_ptr, \
        reset_fn_ptr \
    );

namespace {

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
void __test_dynamic_allocator_get_memory_const_death_if_invalid_input(
    TDynamicAllocator& allocator,
    get_memory_const_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> get_memory_fn_ptr
)
{
    __test_dynamic_allocator_getters_memory_death_if_invalid_input_impl<
        TDynamicAllocator,
        TDynamicAllocatorDataType, 
        decltype(get_memory_fn_ptr)
    >(allocator, get_memory_fn_ptr);
}

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
void __test_dynamic_allocator_get_memory_const_returns_proper_memory(
    TDynamicAllocator& allocator,
    const TDynamicAllocatorDataType& test_value_one,
    const TDynamicAllocatorDataType& test_value_two,
    push_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> push_fn_ptr,
    get_memory_const_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> get_memory_fn_ptr
)
{
    __test_dynamic_allocator_getters_memory_returns_proper_memory_impl<
        TDynamicAllocator,
        TDynamicAllocatorDataType, 
        decltype(get_memory_fn_ptr)
    >(allocator, test_value_one, test_value_two, push_fn_ptr, get_memory_fn_ptr);
}

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
void __test_dynamic_allocator_get_memory_const_death_after_reset_with_index_past_cursor(
    TDynamicAllocator& allocator,
    const TDynamicAllocatorDataType& test_value_one,
    push_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> push_fn_ptr,
    get_memory_const_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> get_memory_fn_ptr,
    reset_dynamic_allocator_fn_signature<TDynamicAllocator> reset_fn_ptr
)
{
    __test_dynamic_allocator_getters_memory_death_after_reset_with_index_past_cursor_impl<
        TDynamicAllocator,
        TDynamicAllocatorDataType, 
        decltype(get_memory_fn_ptr)
    >(
        allocator, 
        test_value_one,
        push_fn_ptr, 
        get_memory_fn_ptr, 
        reset_fn_ptr
    );
}

} // namespace

#define __TEST_DYNAMIC_ALLOCATOR_GET_MEMORY_CONST(Name, Type, test_value_one, test_value_two) \
    __TEST_MAKE_FIXTURE_TEST( \
        DynamicAllocatorGetMemoryConstFixture##Name, \
        DeathIfInvalidInput, \
        (__test_dynamic_allocator_get_memory_const_death_if_invalid_input<DYNAMIC_ALLOCATOR_T(Name), Type>), \
        get_memory_const_fn_ptr \
    ); \
    __TEST_MAKE_FIXTURE_TEST( \
        DynamicAllocatorGetMemoryConstFixture##Name, \
        ReturnsProperMemory, \
        (__test_dynamic_allocator_get_memory_const_returns_proper_memory<DYNAMIC_ALLOCATOR_T(Name), Type>), \
        test_value_one, \
        test_value_two, \
        push_fn_ptr, \
        get_memory_const_fn_ptr \
    ); \
    __TEST_MAKE_FIXTURE_TEST( \
        DynamicAllocatorGetMemoryConstFixture##Name, \
        DeathAfterResetWithIndexPastCursor, \
        (__test_dynamic_allocator_get_memory_const_death_after_reset_with_index_past_cursor<DYNAMIC_ALLOCATOR_T(Name), Type>), \
        test_value_one, \
        push_fn_ptr, \
        get_memory_const_fn_ptr, \
        reset_fn_ptr \
    );

namespace {
    
template <typename TDynamicAllocator>
void __test_dynamic_allocator_reset_death_if_invalid_input(
    reset_dynamic_allocator_fn_signature<TDynamicAllocator> reset_fn_ptr
)
{
    EXPECT_DEATH(reset_fn_ptr(nullptr), "");
}

template <typename TDynamicAllocator>
void __test_dynamic_allocator_reset_valid_reset(
    reset_dynamic_allocator_fn_signature<TDynamicAllocator> reset_fn_ptr
)
{
    TDynamicAllocator allocator{};
    allocator.m_cursor = 100;
    
    reset_fn_ptr(&allocator);
    EXPECT_EQ(allocator.m_cursor, static_cast<size_t>(0));
}

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
void __test_dynamic_allocator_reset_memory_becomes_stale_after_reset(
    TDynamicAllocator& allocator,
    const TDynamicAllocatorDataType& test_value_one,
    const TDynamicAllocatorDataType& test_value_two,
    push_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> push_fn_ptr,
    reset_dynamic_allocator_fn_signature<TDynamicAllocator> reset_fn_ptr
)
{
    auto* stale_ptr = push_fn_ptr(&allocator, &test_value_one);
    reset_fn_ptr(&allocator);
    EXPECT_EQ(allocator.m_cursor, static_cast<size_t>(0));

    auto* new_ptr = push_fn_ptr(&allocator, &test_value_two);
    EXPECT_EQ(allocator.m_cursor, static_cast<size_t>(1));
    EXPECT_EQ(new_ptr, stale_ptr);
    EXPECT_EQ(memcmp(new_ptr, &test_value_two, sizeof(TDynamicAllocatorDataType)), 0);
}

} // namespace

#define __TEST_DYNAMIC_ALLOCATOR_RESET(Name, Type, test_value_one, test_value_two) \
    __TEST_MAKE_ISOLATED_TEST( \
        DynamicAllocatorReset##Name, \
        DeathIfInvalidInput, \
        (__test_dynamic_allocator_reset_death_if_invalid_input<DYNAMIC_ALLOCATOR_T(Name)>(reset_fn_ptr)) \
    ); \
    __TEST_MAKE_ISOLATED_TEST( \
        DynamicAllocatorReset##Name, \
        ValidReset, \
        (__test_dynamic_allocator_reset_valid_reset<DYNAMIC_ALLOCATOR_T(Name)>(reset_fn_ptr)) \
    ); \
    __TEST_MAKE_FIXTURE_TEST( \
        DynamicAllocatorResetFixture##Name, \
        MemoryBecomesStaleAfterReset, \
        (__test_dynamic_allocator_reset_memory_becomes_stale_after_reset<DYNAMIC_ALLOCATOR_T(Name), Type>), \
        test_value_one, \
        test_value_two, \
        push_fn_ptr, \
        reset_fn_ptr \
    );

namespace {
 
template <typename TDynamicAllocator>
void __test_dynamic_allocator_free_death_if_invalid_input(
    free_dynamic_allocator_fn_signature<TDynamicAllocator> free_fn_ptr
)
{
    TDynamicAllocator allocator{};
    EXPECT_DEATH((void)free_fn_ptr(nullptr), "");
    EXPECT_DEATH((void)free_fn_ptr(&allocator), "");
}

template <typename TDynamicAllocator, typename TDynamicAllocatorDataType>
void __test_dynamic_allocator_free_valid_free(
    const TDynamicAllocatorDataType& test_value,
    init_dynamic_allocator_fn_signature<TDynamicAllocator> init_fn_ptr,
    push_dynamic_allocator_fn_signature<TDynamicAllocator, TDynamicAllocatorDataType> push_fn_ptr,
    free_dynamic_allocator_fn_signature<TDynamicAllocator> free_fn_ptr
)
{
    TDynamicAllocator allocator{};
    (void)init_fn_ptr(&allocator, 1);

    push_fn_ptr(&allocator, &test_value);
    EXPECT_NE(allocator.m_memory, nullptr);
    EXPECT_EQ(allocator.m_cursor, static_cast<size_t>(1));

    free_fn_ptr(&allocator);
    EXPECT_EQ(allocator.m_memory, nullptr);
    EXPECT_EQ(allocator.m_cursor, static_cast<size_t>(0));
}

} // namespace

#define __TEST_DYNAMIC_ALLOCATOR_FREE(Name, Type, test_value) \
    __TEST_MAKE_ISOLATED_TEST( \
        DynamicAllocatorFree##Name, \
        DeathIfInvalidInput, \
        (__test_dynamic_allocator_free_death_if_invalid_input<DYNAMIC_ALLOCATOR_T(Name)>(free_fn_ptr)) \
    ); \
    __TEST_MAKE_ISOLATED_TEST( \
        DynamicAllocatorFree##Name, \
        ValidFree, \
        (__test_dynamic_allocator_free_valid_free<DYNAMIC_ALLOCATOR_T(Name), Type>(test_value, init_fn_ptr, push_fn_ptr, free_fn_ptr)) \
    );

#define DYNAMIC_ALLOCATOR_TESTS(Name, Type, test_memory_arr) \
    namespace DynamicAllocatorTests_##Name { \
        static_assert(test_memory_arr.size() == 2, "`test_memory_arr` should have a size of 2"); \
        using TAlloc = DynamicAllocator##Name; \
        class DynamicAllocatorFixture##Name : public DynamicAllocatorBaseFixture<DynamicAllocator##Name> { \
        protected: \
            void SetUp() override { \
                bool r = INIT_DYNAMIC_ALLOCATOR(Name, &allocator, count); \
                ASSERT_TRUE(r); \
            } \
            void TearDown() override { \
                FREE_DYNAMIC_ALLOCATOR(Name, &allocator); \
            } \
        }; \
        class DynamicAllocatorAllocFixture##Name : public DynamicAllocatorFixture##Name {}; \
        class DynamicAllocatorPushFixture##Name : public DynamicAllocatorFixture##Name{}; \
        class DynamicAllocatorGetMemoryFixture##Name : public DynamicAllocatorFixture##Name{}; \
        class DynamicAllocatorGetMemoryConstFixture##Name : public DynamicAllocatorFixture##Name{}; \
        class DynamicAllocatorResetFixture##Name : public DynamicAllocatorFixture##Name{}; \
        __TEST_MAKE_FN_PTRS(Name, Type); \
        __TEST_DYNAMIC_ALLOCATOR_INIT(Name); \
        __TEST_DYNAMIC_ALLOCATOR_REALLOC(Name); \
        __TEST_DYNAMIC_ALLOCATOR_ALLOC(Name, Type); \
        __TEST_DYNAMIC_ALLOCATOR_PUSH(Name, Type, test_memory_arr[0]); \
        __TEST_DYNAMIC_ALLOCATOR_GET_MEMORY(Name, Type, test_memory_arr[0], test_memory_arr[1]); \
        __TEST_DYNAMIC_ALLOCATOR_GET_MEMORY_CONST(Name, Type, test_memory_arr[0], test_memory_arr[1]); \
        __TEST_DYNAMIC_ALLOCATOR_RESET(Name, Type, test_memory_arr[0], test_memory_arr[1]); \
        __TEST_DYNAMIC_ALLOCATOR_FREE(Name, Type, test_memory_arr[0]); \
    } // namespace DynamicAllocatorTests_##Name

namespace {

constexpr std::array __TEST_INT_MEMORY_ARR {
    3,
    913,
};

constexpr std::array __TEST_FLOAT_MEMORY_ARR {
    3.14159f,
    -9.81f,
};

constexpr std::array __TEST_VECTOR3_MEMORY_ARR {
    __Test_Vector3{1.0f, 2.5f, -3.0f},
    __Test_Vector3{0.0f, 0.0f,  1.0f},
};

constexpr std::array __TEST_PLAYER_DATA_MEMORY_ARR {
    __Test_PlayerData{1024, 100, 50},
    __Test_PlayerData{9999, 10,  0},
};

} // namespace

#define __TEST_DYNAMIC_ALLOCATOR_DEF(Name, Type, NameOfArrayContainingTestValues) \
    DYNAMIC_ALLOCATOR_TESTS(Name, Type, NameOfArrayContainingTestValues)
#   include "dynamic_allocator.test.xpp"
#undef __TEST_DYNAMIC_ALLOCATOR_DEF

#undef DYNAMIC_ALLOCATOR_TESTS

#undef __TEST_DYNAMIC_ALLOCATOR_DEF

#undef __TEST_DYNAMIC_ALLOCATOR_FREE
#undef __TEST_DYNAMIC_ALLOCATOR_RESET
#undef __TEST_DYNAMIC_ALLOCATOR_GET_MEMORY_CONST
#undef __TEST_DYNAMIC_ALLOCATOR_GET_MEMORY
#undef __TEST_DYNAMIC_ALLOCATOR_PUSH
#undef __TEST_DYNAMIC_ALLOCATOR_ALLOC
#undef __TEST_DYNAMIC_ALLOCATOR_REALLOC
#undef __TEST_DYNAMIC_ALLOCATOR_INIT

#undef __TEST_MAKE_FIXTURE_TEST
#undef __TEST_MAKE_ISOLATED_TEST
#undef __TEST_MAKE_FN_PTRS
#undef __TEST_MAKE_FREE_DYNAMIC_ALLOCATOR_FN_PTR
#undef __TEST_MAKE_RESET_DYNAMIC_ALLOCATOR_FN_PTR
#undef __TEST_MAKE_GET_MEMORY_CONST_DYNAMIC_ALLOCATOR_FN_PTR
#undef __TEST_MAKE_GET_MEMORY_DYNAMIC_ALLOCATOR_FN_PTR
#undef __TEST_MAKE_PUSH_DYNAMIC_ALLOCATOR_FN_PTR
#undef __TEST_MAKE_ALLOC_DYNAMIC_ALLOCATOR_FN_PTR
#undef __TEST_MAKE_REALLOC_DYNAMIC_ALLOCATOR_FN_PTR
#undef __TEST_MAKE_INIT_DYNAMIC_ALLOCATOR_FN_PTR

#undef __TEST_FREE_DYNAMIC_ALLOCATOR
#undef __TEST_DEFINE_RESET_DYNAMIC_ALLOCATOR
#undef __TEST_DEFINE_GET_MEMORY_CONST_DYNAMIC_ALLOCATOR
#undef __TEST_DEFINE_GET_MEMORY_DYNAMIC_ALLOCATOR
#undef __TEST_DEFINE_PUSH_DYNAMIC_ALLOCATOR
#undef __TEST_DEFINE_ALLOC_DYNAMIC_ALLOCATOR
#undef __TEST_DEFINE_REALLOC_DYNAMIC_ALLOCATOR
#undef __TEST_DEFINE_INIT_DYNAMIC_ALLOCATOR