#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <ostream>
#include <utility>
#include <cstring>


class StringView {
public:
    class ConstIterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = const char;
        using difference_type = std::ptrdiff_t;
        using pointer = const char*;
        using reference = const char&;

    private:
        pointer ptr_;

    public:
        ConstIterator() : ptr_(nullptr) {
        }

        explicit ConstIterator(pointer ptr) : ptr_(ptr) {
        }

        ConstIterator(const ConstIterator& other) = default;
        ConstIterator& operator=(const ConstIterator& other) = default;

        ConstIterator(ConstIterator&& other) = default;
        ConstIterator& operator=(ConstIterator&& other) = default;

        reference operator*() const {
            return *ptr_;
        }

        reference operator[](difference_type idx) const {
            return ptr_[idx];
        }

        pointer operator->() const {
            return ptr_;
        }

        ConstIterator& operator+=(difference_type diff) {
            ptr_ += diff;
            return *this;
        }

        ConstIterator& operator-=(difference_type diff) {
            ptr_ -= diff;
            return *this;
        }

        ConstIterator& operator++() {
            ++ptr_;
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator old = *this;
            ++ptr_;
            return old;
        }

        ConstIterator& operator--() {
            --ptr_;
            return *this;
        }

        ConstIterator operator--(int) {
            ConstIterator old = *this;
            --ptr_;
            return old;
        }

        ConstIterator operator+(difference_type diff) const {
            return ConstIterator(ptr_ + diff);
        }

        ConstIterator operator-(difference_type diff) const {
            return ConstIterator(ptr_ - diff);
        }

        difference_type operator-(const ConstIterator& other) const {
            return ptr_ - other.ptr_;
        }

        bool operator<(const ConstIterator& other) const {
            return ptr_ < other.ptr_;
        }

        bool operator<=(const ConstIterator& other) const {
            return ptr_ <= other.ptr_;
        }

        bool operator>(const ConstIterator& other) const {
            return ptr_ > other.ptr_;
        }

        bool operator>=(const ConstIterator& other) const {
            return ptr_ >= other.ptr_;
        }

        bool operator==(const ConstIterator& other) const {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const ConstIterator& other) const {
            return ptr_ != other.ptr_;
        }

        friend inline ConstIterator operator+(difference_type diff, const ConstIterator& iter);
    };

    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

private:
    const char* string_;
    size_t size_;

public:
    StringView() : string_(nullptr), size_(0) {
    }

    StringView(const char* str) : string_(str), size_(strlen(str)) {
    }

    StringView(const char* str, size_t size) : string_(str), size_(size) {
    }

    char operator[](size_t idx) const {
        return string_[idx];
    }

    char At(size_t idx) const {
        if (idx >= size_) {
            throw std::out_of_range("String index out of range");
        }
        return string_[idx];
    }

    char Front() const {
        return string_[0];
    }

    char Back() const {
        return string_[size_ - 1];
    }

    size_t Size() const {
        return size_;
    }

    size_t Length() const {
        return size_;
    }

    bool Empty() const {
        return size_ == 0;
    }

    const char* Data() const {
        return string_;
    }

    void Swap(StringView& other) {
        std::swap(string_, other.string_);
        std::swap(size_, other.size_);
    }

    void RemovePrefix(size_t prefix_size) {
        size_t offset = std::min(prefix_size, size_);
        string_ += offset;
        size_ -= offset;
    }

    void RemoveSuffix(size_t suffix_size) {
        size_t offset = std::min(suffix_size, size_);
        size_ -= offset;
    }

    StringView Substr(size_t pos, size_t count = -1) {
        if (size_ < pos || count == 0) {
            return StringView();
        } else {
            return StringView(string_ + pos, std::min(count, size_ - pos));
        }
    }

    ConstIterator begin() const {
        return ConstIterator(string_);
    }

    ConstIterator end() const {
        return ConstIterator(string_ + size_);
    }

    ConstIterator cbegin() const {
        return begin();
    }

    ConstIterator cend() const {
        return end();
    }

    ConstReverseIterator rbegin() const {
        return ConstReverseIterator(end());
    }

    ConstReverseIterator rend() const {
        return ConstReverseIterator(begin());
    }

    ConstReverseIterator crbegin() const {
        return rbegin();
    }

    ConstReverseIterator crend() const {
        return rend();
    }
};

inline StringView::ConstIterator operator+(
        StringView::ConstIterator::difference_type diff,
        const StringView::ConstIterator& iter
) {
    return iter + diff;
}

#endif // STRING_VIEW_H
