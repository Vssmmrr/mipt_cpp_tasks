#ifndef DEQUE_H
#define DEQUE_H

#include <iostream>
#include <cstdint>

template <class T, size_t Capacity>
class Page {
private:
    T buffer_[Capacity];
    size_t size_ = 0;
    size_t head_ = 0;

public:
    Page() : size_(0), head_(0) {
    }

    Page(const Page& other) = default;

    Page& operator=(const Page& other) = default;

    ~Page() = default;

    T& operator[](size_t idx) {
        return buffer_[head_ + idx];
    }

    T operator[](size_t idx) const {
        return buffer_[head_ + idx];
    }

    T& Front() {
        return buffer_[head_];
    }

    T& Back() {
        return buffer_[head_ + size_ - 1];
    }

    T Front() const {
        return buffer_[head_];
    }

    T Back() const {
        return buffer_[head_ + size_ - 1];
    }

    bool Empty() const {
        return size_ == 0;
    }

    bool Full() const {
        return size_ == Capacity;
    }

    size_t Size() const {
        return size_;
    }

    void Clear() {
        size_ = 0;
        head_ = 0;
    }

    bool IsFront() const {
        if (size_ == 0) {
            return true;
        }
        return (head_ > 0);
    }

    bool IsBack() const {
        if (size_ == 0) {
            return true;
        }
        return (head_ + size_ < Capacity);
    }

    void PushBack(const T& value) {
        if (IsBack()) {
            if (size_ == 0) {
                head_ = 0;
            }
            buffer_[head_ + size_] = value;
            ++size_;
        }
    }

    void PushFront(const T& value) {
        if (IsFront()) {
            if (size_ == 0) {
                head_ = Capacity;
            }
            --head_;
            buffer_[head_] = value;
            ++size_;
        }
    }

    void PopBack() {
        --size_;
    }

    void PopFront() {
        ++head_;
        --size_;
    }
};

template <class T>
inline void Fill(T* begin, T* end, const T& value) {
    for (T* ptr = begin; ptr < end; ++ptr) {
        *ptr = value;
    }
}

template  <class T>
inline void Copy(const T* source, T* destination, size_t copy_size, size_t head, size_t capacity) {
    for(size_t i = 0; i < copy_size; ++i) {
        destination[i] = source[(i + head) % capacity];
    }
}

template <class T>
class CircularBuffer {
private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    int head_ = 0;
    int tail_ = 0;
    T* buffer_ = nullptr;

    const static size_t kIncreaseFactor = 2;

    void Grow() {
        size_t new_cap = capacity_ * kIncreaseFactor;
        if (new_cap == 0) {
            new_cap = 1;
        }
        Reallocate(new_cap);
    }

    void Reallocate(size_t new_cap) {
        T* tmp = new T[new_cap];
        ::Copy(buffer_, tmp, size_, head_, capacity_);
        delete[] buffer_;
        capacity_ = new_cap;
        buffer_ = tmp;
        head_ = 0;
        tail_ = (size_ == 0) ? 0 : size_ - 1;
    }

    void Copy(const CircularBuffer& other) {
        Reallocate(other.size_);
        size_ = other.size_;
        head_ = 0;
        tail_ = size_ - 1;
        ::Copy(other.buffer_, buffer_, size_, other.head_, other.capacity_);
    }

public:
    CircularBuffer() : size_(0), capacity_(0), head_(0), tail_(0), buffer_(nullptr) {
    }

    explicit CircularBuffer(size_t count) : CircularBuffer(count, T()) {
    }

    CircularBuffer(size_t size, const T& value) : size_(size), capacity_ (size), head_(0), tail_(size - 1) {
        buffer_ = new T[capacity_];
        Fill(buffer_, buffer_ + size_, value);
    }

    CircularBuffer(const CircularBuffer& other) : CircularBuffer() {
        Copy(other);
    }

    T& operator=(const CircularBuffer& other) {
        if (&other == this) {
            return *this;
        }
        Clear();
        Copy(other);
        return *this;
    }

    ~CircularBuffer() {
        delete[] buffer_;
    }

    void Clear() {
        size_ = 0;
        head_ = 0;
        tail_ = 0;
    }

    bool Empty() const {
        return size_ == 0;
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    T operator[](size_t idx) const {
        return buffer_[(head_ + idx) % capacity_];
    }

    T& operator[](size_t idx) {
        return buffer_[(head_ + idx) % capacity_];
    }

    T& Front() {
        return buffer_[head_];
    }

    T Front() const {
        return buffer_[head_];
    }

    T& Back() {
        return buffer_[tail_];
    }

    T Back() const {
        return buffer_[tail_];
    }

    void PushFront(const T& value) {
        if (size_ == capacity_) {
            Grow();
        }
        if (size_ != 0) {
            head_ = (head_ - 1 + capacity_) % capacity_;
        }
        ++size_;
        buffer_[head_] = value;
    }

    void PushBack(const T& value) {
        if (size_ == capacity_) {
            Grow();
        }
        if (size_ != 0) {
            tail_ = (tail_ + 1) % capacity_;
        }
        ++size_;
        buffer_[tail_] = value;
    }

    void Reserve(size_t new_cap) {
        if (new_cap <= capacity_) {
            return;
        }
        Reallocate(new_cap);
    }

    void PopBack() {
        if (size_ > 1) {
            tail_ = (tail_ - 1 + capacity_) % capacity_;
        }
        --size_;
    }

    void PopFront() {
        if (size_ != 1) {
            head_ = (head_ + 1) % capacity_;
        }
        --size_;
    }

    void Swap(CircularBuffer& other) {
        std::swap(buffer_, other.buffer_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
    }
};

template <class T>
void RotateLeft(T* array, size_t size, size_t rotate_count) {
    size_t source_start = rotate_count;
    for (size_t i = 0, cur = source_start; i < cur; ++i) {
        std::swap(array[i], array[cur]);
        ++cur;
        if (cur == size) {
            cur = source_start;
        } else if (i == source_start - 1) {
            source_start = cur;
        }
    }
}

template <class T, size_t PageSize = 100>
class Deque {
private:
    typedef Page<T, PageSize> PageType;

    CircularBuffer<PageType*> pages_;
    size_t used_head_ = 0;
    size_t used_tail_ = 0;
    size_t size_ = 0;

    static const size_t kIncreaseFactor = 2;

    size_t UsedPagesCount() const {
        if (pages_.Empty()) {
            return 0;
        }
        return (used_tail_ + pages_.Size() - used_head_) % pages_.Size() + 1;
    }

    void Copy(const Deque& other) {
        Clear();
        for (size_t i = 0; i < other.UsedPagesCount(); ++i) {
            EnsureSpaceBack();
            *pages_[used_tail_] = *other.pages_[(other.used_head_ + i) % other.pages_.Size()];
        }
        size_ = other.size_;
    }

    void AlignHead() {
        if (used_head_ == 0 || pages_.Empty()) {
            return;
        }
        RotateLeft(&pages_[0], pages_.Size(), used_head_);
        used_tail_ = UsedPagesCount() - 1;
        used_head_ = 0;
    }

    void Grow() {
        AlignHead();
        size_t new_pages_cnt = pages_.Size() * kIncreaseFactor;
        if (new_pages_cnt == 0) {
            new_pages_cnt = 1;
        }
        while (pages_.Size() < new_pages_cnt) {
            pages_.PushBack(new PageType);
        }
    }

    void EnsureSpaceFront() {
        if (pages_.Empty() || !pages_[used_head_]->IsFront()) {
            if (UsedPagesCount() == pages_.Size()) {
                Grow();
            }
            used_head_ = (used_head_ + pages_.Size() - 1) % pages_.Size();
            pages_[used_head_]->Clear();
        }
    }

    void EnsureSpaceBack() {
        if (pages_.Empty() || !pages_[used_tail_]->IsBack()) {
            if (UsedPagesCount() == pages_.Size()) {
                Grow();
            }
            used_tail_ = (used_tail_ + 1) % pages_.Size();
            pages_[used_tail_]->Clear();
        }
    }

public:
    Deque() = default;

    ~Deque() {
        for (size_t i = 0; i < pages_.Size(); ++i) {
            delete pages_[i];
        }
    }

    Deque(const Deque& other) {
        Copy(other);
    }

    Deque& operator=(const Deque& other) {
        if (this != &other) {
            Copy(other);
        }
        return *this;
    }

    void Swap(Deque& other) {
        if (this == &other) {
            return;
        }
        pages_.Swap(other.pages_);
        std::swap(size_, other.size_);
    }

    T& operator[](size_t idx) {
        size_t first_size = pages_[used_head_]->Size();
        if (first_size > idx) {
            return (*pages_[used_head_])[idx];
        }
        size_t page_no = (idx - first_size) / PageSize + 1;
        size_t page_offset = (idx - first_size) % PageSize;
        return (*pages_[(page_no + used_head_) % pages_.Size()])[page_offset];
    }

    T operator[](size_t idx) const {
        size_t first_size = pages_[used_head_]->Size();
        if (first_size > idx) {
            return (*pages_[used_head_])[idx];
        }
        size_t page_no = (idx - first_size) / PageSize + 1;
        size_t page_offset = (idx - first_size) % PageSize;
        return (*pages_[(page_no + used_head_) % pages_.Size()])[page_offset];
    }

    T& Front() {
        return pages_[used_head_]->Front();
    }

    T Front() const {
        return pages_[used_head_]->Front();
    }

    T& Back() {
        return pages_[used_tail_]->Back();
    }

    T Back() const {
        return pages_[used_tail_]->Back();
    }

    size_t Size() const {
        return size_;
    }

    bool Empty() const {
        return size_ == 0;
    }

    void PushBack(const T& value) {
        EnsureSpaceBack();
        pages_[used_tail_]->PushBack(value);
        ++size_;
    }

    void PopBack() {
        if (!Empty()) {
            pages_[used_tail_]->PopBack();
            if (pages_[used_tail_]->Empty() && UsedPagesCount() > 1) {
                used_tail_ = (used_tail_ + pages_.Size() - 1) % pages_.Size();
            }
            --size_;
        }
    }

    void PushFront(const T& value) {
        EnsureSpaceFront();
        pages_[used_head_]->PushFront(value);
        ++size_;
    }

    void PopFront() {
        if (size_ > 0) {
            pages_[used_head_]->PopFront();
            if (pages_[used_head_]->Empty() && UsedPagesCount() > 1) {
                used_head_ = (used_head_ + 1) % pages_.Size();
            }
            --size_;
        }
    }

    void Clear() {
        size_ = 0;
        used_tail_ = 0;
        used_head_ = 0;
        if (!pages_.Empty()) {
            pages_[0]->Clear();
        }
    }
};

#endif // DEQUE_H