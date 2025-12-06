#ifndef YDS_ALLOCATOR_H
#define YDS_ALLOCATOR_H

#include <cstdlib>

#if defined(_MSC_VER)
#include <malloc.h>    // _aligned_malloc, _aligned_free
#endif

class ysAllocator {
public:
    // Primary template: aligned allocation
    template <int Alignment>
    static void *BlockAllocate(int size);

    // Free function
    static void BlockFree(void *block, int alignment);

    // Allocate typed objects (optionally calling constructors)
    template <typename T_Create, int Alignment>
    static T_Create *TypeAllocate(int n = 1, bool construct = true);

    // Free typed objects (optionally calling destructors)
    template <typename T_Free>
    static void TypeFree(T_Free *data, int n = 1, bool destroy = true, int alignment = 1);
};

template <int Alignment>
void *ysAllocator::BlockAllocate(int size) {
#if defined(_MSC_VER)
    // Windows/MSVC: aligned malloc
    return ::_aligned_malloc(size, Alignment);
#else
    // Linux/macOS: POSIX memalign
    void *ptr = nullptr;
    // posix_memalign requires alignment to be a multiple of sizeof(void*)
    size_t effectiveAlignment = Alignment < sizeof(void*) ? sizeof(void*) : Alignment;
    if (posix_memalign(&ptr, effectiveAlignment, size) != 0)
        return nullptr;
    return ptr;
#endif
}

//
// Explicit specialization for Alignment = 1
// Just use regular malloc
//
template <>
inline void *ysAllocator::BlockAllocate<1>(int size) {
    return ::malloc(size);
}

//
// Free
//
inline void ysAllocator::BlockFree(void *block, int alignment) {
#if defined(_MSC_VER)
    if (alignment != 1)
        ::_aligned_free(block);
    else
        ::free(block);
#else
    // On POSIX, posix_memalign allocations must be freed with free()
    ::free(block);
#endif
}

template <typename T_Create, int Alignment>
T_Create *ysAllocator::TypeAllocate(int n, bool construct) {
    void *block = BlockAllocate<Alignment>(sizeof(T_Create) * n);
    T_Create *typedArray = reinterpret_cast<T_Create *>(block);

    if (construct) {
        for (int i = 0; i < n; i++) {
            new(typedArray + i) T_Create;
        }
    }

    return typedArray;
}

template <typename T_Free>
void ysAllocator::TypeFree(T_Free *data, int n, bool destroy, int alignment) {
    if (destroy) {
        for (int i = 0; i < n; i++) {
            data[i].~T_Free();
        }
    }

    BlockFree(reinterpret_cast<void *>(data), alignment);
}

// class ysAllocator {
// public:
//     template <int Alignment>
//     static void *BlockAllocate(int size) {
//         return ::_aligned_malloc(size, Alignment);
//     }

//     template <>
//     static void *BlockAllocate<1>(int size) {
//         return ::malloc(size);
//     }

//     static void BlockFree(void *block, int alignment) {
//         if (alignment != 1) {
//             ::_aligned_free(block);
//         }
//         else {
//             ::free(block);
//         }
//     }

// template <typename T_Create, int Alignment>
// static T_Create *TypeAllocate(int n = 1, bool construct = true) {
//     void *block = BlockAllocate<Alignment>(sizeof(T_Create) * n);
//     T_Create *typedArray = reinterpret_cast<T_Create *>(block);

//     if (construct) {
//         for (int i = 0; i < n; i++) {
//             new(typedArray + i) T_Create;
//         }
//     }

//     return typedArray;
// }

// template <typename T_Free>
// static void TypeFree(T_Free *data, int n = 1, bool destroy = true, int alignment = 1) {
//     void *block = reinterpret_cast<void *>(data);

//     if (destroy) {
//         for (int i = 0; i < n; i++) {
//             data[i].~T_Free();
//         }
//     }

//     BlockFree(block, alignment);
// }
// };

#endif /* YDS_ALLOCATOR_H */
