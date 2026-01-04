/**
 * @file arena.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief Memory Arena API
 * 
 * This file defines a simple memory arena (region-based memory management)
 * optimized for fast allocation of many small objects with similar lifetimes.
 * @version 1.0.1
 * @date 2026-01-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace json {

/**
 * @brief A memory arena for efficient allocation of small objects.
 * 
 * The Arena class manages memory in large blocks. Allocations are extremely fast
 * (pointer bump) and deallocation happens all at once when the Arena is destroyed.
 * This is ideal for constructing ASTs where nodes are allocated sequentially
 * and destroyed together.
 */
class Arena {
public:
    /// Default size for memory blocks (64KB).
    static constexpr size_t DefaultBlockSize = 64 * 1024;

    /**
     * @brief Constructs a new Arena.
     * 
     * @param block_size The size of each memory block in bytes. Defaults to 64KB.
     */
    explicit Arena(size_t block_size = DefaultBlockSize)
        : block_size_(block_size), current_(nullptr), remaining_(0) {
        allocate_block();
    }

    /**
     * @brief Destroys the Arena and frees all allocated memory blocks.
     * 
     * Note: Destructors of objects allocated in the arena are NOT called.
     * This is suitable for POD types or types where destruction is not strictly required.
     */
    ~Arena() {
        for (auto* block : blocks_) {
            ::operator delete(block);
        }
    }

    // Non-copyable, non-movable
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    /**
     * @brief Allocates memory for objects of type T.
     * 
     * Handles alignment requirements for type T.
     * 
     * @tparam T The type of object to allocate.
     * @param count The number of objects to allocate. Defaults to 1.
     * @return T* Pointer to the allocated memory.
     */
    template<typename T>
    [[nodiscard]] T* alloc(size_t count = 1) {
        constexpr size_t alignment = alignof(T);
        size_t size = sizeof(T) * count;
        
        // Align current pointer
        uintptr_t addr = reinterpret_cast<uintptr_t>(current_);
        uintptr_t aligned = (addr + alignment - 1) & ~(alignment - 1);
        size_t padding = aligned - addr;

        if (padding + size > remaining_) [[unlikely]] {
            if (size > block_size_) {
                // Large allocation - dedicated block
                void* large = ::operator new(size);
                blocks_.push_back(static_cast<uint8_t*>(large));
                return static_cast<T*>(large);
            }
            allocate_block();
            return alloc<T>(count);
        }

        current_ += padding;
        remaining_ -= padding;

        T* result = reinterpret_cast<T*>(current_);
        current_ += size;
        remaining_ -= size;

        return result;
    }

    /**
     * @brief Resets the arena, invalidating all previous allocations.
     * 
     * This allows reusing the allocated blocks for new data without
     * the cost of freeing and re-allocating memory from the OS.
     */
    void reset() {
        if (!blocks_.empty()) {
            current_ = blocks_[0];
            remaining_ = block_size_;
        }
    }

private:
    /// Allocates a new memory block from the system.
    void allocate_block() {
        void* block = ::operator new(block_size_);
        blocks_.push_back(static_cast<uint8_t*>(block));
        current_ = static_cast<uint8_t*>(block);
        remaining_ = block_size_;
    }

    size_t block_size_;
    uint8_t* current_;
    size_t remaining_;
    std::vector<uint8_t*> blocks_;
};

} // namespace json
