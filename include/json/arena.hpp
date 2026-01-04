#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace json {

class Arena {
public:
    static constexpr size_t DefaultBlockSize = 64 * 1024; // 64KB

    explicit Arena(size_t block_size = DefaultBlockSize)
        : block_size_(block_size), current_(nullptr), remaining_(0) {
        allocate_block();
    }

    ~Arena() {
        for (auto* block : blocks_) {
            ::operator delete(block);
        }
    }

    // Non-copyable, non-movable
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

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

    void reset() {
        if (!blocks_.empty()) {
            current_ = blocks_[0];
            remaining_ = block_size_;
        }
    }

private:
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
