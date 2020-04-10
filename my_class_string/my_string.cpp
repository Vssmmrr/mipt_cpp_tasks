#include "my_string.h"
#include <iostream>

void String::Copy(size_t size, const char* source, char* destination) {
    for (size_t i = 0; i < size; ++i) {
        destination[i] = source[i];
    }
}

void String::IncreaseMemoryAmount(size_t expected_capacity) {
    if (expected_capacity <= capacity_) {
        return;
    }
    if (capacity_ == 0) {
        capacity_ = 1;
    }
    while (capacity_ < expected_capacity) {
        capacity_ *= kIncreaseFactor;
    }
    char* tmp = new char[capacity_];
    Copy(size_,buffer_,tmp);
    tmp[size_] = '\0';
    delete[] buffer_;
    buffer_ = tmp;
}

void String::Copy(const String& other) {
    Clear();
    IncreaseMemoryAmount(other.size_ + 1);
    size_ = other.size_;
    Copy(size_ + 1, other.buffer_, buffer_);
}

String::String() : size_(0), capacity_(1) {
    buffer_ = new char[capacity_];
    buffer_[size_] = '\0';
}

String::String(const char* str) : String(str, CountLength(str)) {
}

String::String(const char* str, size_t n) {
    size_ = n;
    capacity_ = size_ + 1;
    buffer_ = new char[capacity_];
    Copy(size_, str, buffer_);
    buffer_[size_] = '\0';
}

void String::Fill( size_t count, char symbol, char* destination) {
    for (size_t i = 0; i < count; ++i) {
        destination[i] = symbol;
    }
}

String::String(size_t size, char symbol) : String() {
    size_ = size;
    capacity_ = size_ + 1;
    Fill( size_, symbol, buffer_);
    buffer_[size_] = '\0';
}

String::String(const String& other) : String() {
    Copy(other);
}

String& String::operator=(const String& other) {
    if (this == &other) {
        return *this;
    }
    Copy(other);
    return *this;
}

String::~String() {
    delete[] buffer_;
}

size_t String::Size() const {
    return size_;
}

size_t String::Length() const {
    return size_;
}

size_t String::Capacity() const {
    return capacity_;
}

void String::Resize(size_t new_size, char fill) {
    if (new_size <= size_) {
        size_ = new_size;
        buffer_[size_] = '\0';
    } else {
        IncreaseMemoryAmount(new_size + 1);
        for (size_t i = size_; i < new_size; ++i) {
            buffer_[i] = fill;
        }
        buffer_[new_size] = '\0';
        size_ = new_size;
    }
}

void String::Reserve(size_t new_capacity) {
    IncreaseMemoryAmount(new_capacity);
}

void String::ShrinkToFit() {
    if (capacity_ == size_ + 1) {
        return;
    }
    char* tmp = new char[size_ + 1];
    Copy(size_, buffer_, tmp);
    tmp[size_] = '\0';
    capacity_ = size_ + 1;
    delete[] buffer_;
    buffer_ = tmp;
}

bool String::Empty() const {
    return size_ == 0;
}

void String::Clear() {
    size_ = 0;
    buffer_[size_] = '\0';
}

char& String::Back() {
    return buffer_[size_ - 1];
}
char& String::Front() {
    return buffer_[0];
}

const char& String::Back() const {
    return buffer_[size_ - 1];
}

const char& String::Front() const {
    return buffer_[0];
}

const char* String::CStr() const {
    return buffer_;
}

const char* String::Data() const {
    return buffer_;
}

String& String::operator+=(const String& other) {
    IncreaseMemoryAmount(size_ + other.size_ + 1);
    Copy(other.size_, other.buffer_, buffer_ + size_);
    size_ += other.size_;
    buffer_[size_] = '\0';
    return *this;
}

String& String::operator+=(char symbol) {
    //return *this += String(1, symbol);
    IncreaseMemoryAmount(size_ + 2);
    buffer_[size_] = symbol;
    ++size_;
    buffer_[size_] = '\0';
    return *this;
}

void String::PushBack(char symbol) {
    *this += symbol;
}

char String::operator[](size_t idx) const {
    return buffer_[idx];
}

char& String::operator[](size_t idx) {
    return buffer_[idx];
}

String operator+(const String& lhs, const String& rhs) {
    String s = lhs;
    s += rhs;
    return s;
}

bool operator>(const String& lhs, const String& rhs) {
    for (size_t i = 0; i < lhs.Size() && i < rhs.Size(); ++i) {
        if (lhs[i] > rhs[i]) {
            return true;
        } else if (lhs[i] < rhs[i]) {
            return false;
        }
    }
    return lhs.Size() > rhs.Size();
}

bool operator<(const String& lhs, const String& rhs) {
    return rhs > lhs;
}

bool operator<=(const String& lhs, const String& rhs) {
    return !(lhs > rhs);
}

bool operator>=(const String& lhs, const String& rhs) {
    return !(lhs < rhs);
}

bool operator==(const String& lhs, const String& rhs) {
    if (lhs.Size() != rhs.Size()) {
        return false;
    } else {
        for (size_t i = 0; i < lhs.Size(); ++i) {
            if (lhs[i] != rhs[i]) {
                return false;
            }
        }
        return true;
    }
}

bool operator!=(const String& lhs, const String& rhs) {
    return !(lhs == rhs);
}

std::istream& operator>>(std::istream& is, String& input) {
    input.Clear();
    int symbol = is.get();
    while (!isspace(symbol) && !is.eof()) {
        input.PushBack(symbol);
        symbol = is.get();
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const String& output) {
//    for (size_t i = 0; i < output.Size(); ++i){
//        os << output[i];
//    }
    if (output.Size() > 0) {
        os << output.Data();
    }
    return os;
}

size_t CountLength(const char* str) {
    size_t length = 0;
    for (size_t i = 0; str[i] != '\0'; ++i) {
        ++length;
    }
    return length;
}