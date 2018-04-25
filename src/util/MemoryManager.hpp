/*
 * MemoryManager.hpp
 *
 * Created on 2013-09-22
 */

#ifndef UTIL_MEMORYMANAGER_HPP
#define	UTIL_MEMORYMANAGER_HPP

#pragma GCC diagnostic ignored "-Wunused-parameter"

namespace util {

/**
 * MemoryManager is a memory manager for objects of type T in a single-threaded
 * environment.
 *
 * A memory manager maintains a pool of T objects and allocates T objects from
 * this pool. If the number of T objects that need to be created exceeds the
 * number of objects in the pool, the pool is expanded. Deleted objects are
 * returned to this pool. Note that the pool never shrinks.
 *
 * Specification Fields:
 *   - pool : FreeList // The pool of free memory blocks.
 *
 * Abstract Invariant:
 *   Each memory block in the pool has a size >= to the size of a T object.
 *
 * @author mwi
 */
template <typename T>
class MemoryManager {

private:
    /* Free list of memory. */
    struct FreeList {
        FreeList * next;
    };

    /* If the free list is empty, expand it by this amount. */
    enum { EXPANSION_SIZE = 32 };

    /* The head of the free list. */
    FreeList * freeListHead;

public:
    /**
     * @requires size > 0
     * @effects Makes this be a new memory manager with a pool of initial size
     *           'size'.
     */
    MemoryManager(size_t size = EXPANSION_SIZE);

    /**
     * @effects Destroys this.
     */
    ~MemoryManager();

    /**
     * @modifies this
     * @effects Allocates an object of type T from this.pool.
     */
    void * allocate(size_t size);

    /**
     * @requires element != null && element is allocated from this.pool
     * @modifies this
     * @effects Frees 'element', i.e., adds 'element' to this.pool.
     */
    void free(void * deleted);

private:
    /**
     * @requires size > 0
     * @modifies this
     * @effects Expands this.pool by 'howMany' memory blocks.
     */
    void expandPool(int howMany = EXPANSION_SIZE);
};

template<typename T>
MemoryManager<T>::MemoryManager(size_t size) : freeListHead(0) {
    expandPool(size);
}

template<typename T>
MemoryManager<T>::~MemoryManager() {
    FreeList * nextPtr = freeListHead;
    for (; nextPtr; nextPtr = freeListHead) {
        freeListHead = freeListHead->next;
        delete [] nextPtr;
    }
}

template<typename T>
inline void * MemoryManager<T>::allocate(size_t size) {
    if (!freeListHead) {
        expandPool();
    }

    FreeList * head = freeListHead;
    freeListHead = head->next;

    return head;
}

template<typename T>
inline void MemoryManager<T>::free(void * element) {
    FreeList * head = static_cast<FreeList *>(element);
    head->next = freeListHead;
    freeListHead = head;
}

template<typename T>
void MemoryManager<T>::expandPool(int howMany) {
    size_t size = (sizeof(T) > sizeof(FreeList *)) ? sizeof(T)
            : sizeof(FreeList *);

    FreeList * head = reinterpret_cast<FreeList *>(new char[size]);
    freeListHead = head;

    for (int i = 0; i < howMany; ++i) {
        head->next = reinterpret_cast<FreeList *>(new char[size]);
        head = head->next;
    }

    head->next = 0;
}

} // namespace util

#endif	/* UTIL_MEMORYMANAGER_HPP */
