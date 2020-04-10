#ifndef MY_CLASS_STRING_MY_STRING_H
#define MY_CLASS_STRING_MY_STRING_H

#include <iostream>
#include <cstddef>

class String {
    char* buffer_;
    size_t size_;
    size_t capacity_;

    const static size_t kIncreaseFactor = 2;

    void Copy(const String& other);
    static void Fill( size_t count, char symbol, char* destination);
    static void Copy(size_t size, const char* source, char* destination);
    void IncreaseMemoryAmount(size_t expected_capacity);

public:
    String();
    String(const char* str);
    String(const char* str, size_t n);
    String(size_t size, char symbol);

    String(const String& other);
    String& operator=(const String& other);
    ~String();

    size_t Size() const;
    size_t Length() const;
    size_t Capacity() const;
    void Resize(size_t new_size, char fill = char());
    bool Empty() const;
    void Clear();
    void Reserve(size_t new_capacity);
    void ShrinkToFit();

    char& Back();
    char& Front();

    char operator[](size_t idx) const;
    char& operator[](size_t idx);

    const char& Back() const;
    const char& Front() const;

    const char* CStr() const;
    const char* Data() const;

    String& operator+=(const String& other);
    String& operator+=(char symbol);

    void PushBack(char symbol);
};

String operator+(const String& lhs, const String& rhs);

bool operator>(const String& lhs, const String& rhs);
bool operator<(const String& lhs, const String& rhs);
bool operator==(const String& lhs, const String& rhs);
bool operator!=(const String& lhs, const String& rhs);
bool operator>=(const String& lhs, const String& rhs);
bool operator<=(const String& lhs, const String& rhs);

std::istream& operator>>(std::istream& is, String& input);
std::ostream& operator<<(std::ostream& os, const String& output);

size_t CountLength(const char* str);

#endif //MY_CLASS_STRING_MY_STRING_H