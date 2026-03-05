#pragma once
#include <vector>
#include <cassert>

template<typename T>
class RingBuffer
{
public:
    RingBuffer() {}
    //explicit prevents accidental implicit assigning of data with syntax like
    //RingBuffer<float> buffer = 512; and instead forces use of syntax like
    //RingBuffer<float> buffer(512); so initialisation is more intentional
    explicit RingBuffer(size_t capacity)
        : buffer_(capacity, T{})
    {
        assert(capacity > 0);
    }

    //noexcept forces no exceptions which is important for optimisation especially
    //in audio related contexts
    size_t capacity() const noexcept
    {
        return buffer_.size();
    }

    // write at current head and increment head
    void write(const T& value) noexcept
    {
        buffer_[head_] = value;
        head_ = increment(head_);
    }

    //read relative to head position (use of asserts is good practice in audio
    //because they are optimised out in release builds I believe?)
    T readRelative(size_t offset) const noexcept
    {
        assert(offset < capacity());

        size_t index = (head_ + capacity() - 1 - offset) % capacity();

        return buffer_[index];
    }

    //direct access to buffer data;
    T& operator[](size_t index) noexcept
    {
        return buffer_[index];
    }

    //apparently good practice to put const versions of operators into types
    //allows differentiation in function calls between const and non const 
    //versions of class in code
    const T& operator[](size_t index) const noexcept
    {
        return buffer_[index];
    }

    //expose head in case it is wanted for iteration
    size_t head() const noexcept
    {
        return head_;
    }

private:
    //quick little increment function for head iteration
    size_t increment(size_t i) const noexcept
    {
        return (i + 1) % capacity();
    }

    std::vector<T> buffer_;
    size_t head_ = 0;

    
};