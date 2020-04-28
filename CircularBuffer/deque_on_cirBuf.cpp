#include <iostream>
#include <cstring>
#include <utility>

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

    CircularBuffer(size_t size, const T& value) : size_(size), capacity_ (size), head_(0), tail_(0) {
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
        buffer_[head_] = value;
        ++size_;
    }

    void PushBack(const T& value) {
        if (size_ == capacity_) {
            Grow();
        }
        if (size_ != 0) {
            tail_ = (tail_ + 1) % capacity_;
        }
        buffer_[tail_] = value;
        ++size_;
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

int main() {

    CircularBuffer<int> deq;

    char* command = new char[30];

    std::cin >> command;

    while(strcmp(command, "exit") != 0) {

        if (strcmp(command, "size") == 0) {
            std::cout << deq.Size() << "\n";
        }
        else if (strcmp(command, "pop_back") == 0) {
            if (!deq.Empty()) {
                std::cout << deq.Back() << '\n';
                deq.PopBack();
            } else {
                std::cout << "error\n";
            }
        }
        else if (strcmp(command, "pop_front") == 0) {
            if (!deq.Empty()) {
                std::cout << deq.Front() << '\n';
                deq.PopFront();
            } else {
                std::cout << "error\n";
            }
        }
        else if (strcmp(command, "push_front") == 0) {
            int arg;
            std::cin >> arg;
            deq.PushFront(arg);
            std::cout << "ok\n";
        }
        else if (strcmp(command, "push_back") == 0) {
            int arg;
            std::cin >> arg;
            deq.PushBack(arg);
            std::cout << "ok\n";
        }
        else if (strcmp(command, "front") == 0) {
            if (!deq.Empty()) {
                std::cout << deq.Front() << '\n';
            } else {
                std::cout << "error\n";
            }
        }
        else if (strcmp(command, "back") == 0) {
            if (!deq.Empty()) {
                std::cout << deq.Back() << '\n';
            } else {
                std::cout << "error\n";
            }
        }
        else if (strcmp(command, "clear") == 0) {
            deq.Clear();
            std::cout << "ok\n";
        }

        std::cin >> command;
    }

    std::cout << "bye\n";

    delete[] command;

    return 0;
}

