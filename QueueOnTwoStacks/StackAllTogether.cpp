#include <iostream>
#include <cstddef>
#include <cstring>
#include <cctype>
typedef int T;

struct Node {
    Node* prev_;
    T value_;

    Node(Node* prev, T value): prev_(prev), value_(value){
    }

};

class Stack {
private:
    Node* head_;
    size_t size_;

public:
    void Push(T value) {
        Node* newPrev = new Node(head_, value);
        head_ = newPrev;
        ++size_;
    }

    void Pop() {
        if (Empty()){
            return;
        }
        Node* newTop = head_->prev_;
        delete head_;
        head_ = newTop;
        --size_;
    }

    T Top() const {
        if (!Empty()){
            return head_->value_;
        }
    }

    /*
    T& Top() {
        if (!Empty()){
            return head_->value_;
        }
    } */

    void Clear() {
        while (!Empty()){
            Pop();
        }
    }

    bool Empty() const {
        return size_ == 0;
    }

    size_t Size() const {
        return size_;
    }

    void Copy(const Stack& other) {
        this->Clear();
        //this->size_ = other.size_;

        T* values = new T[size_];
        Node* element = other.head_;
        for(size_t i = 0; i < other.size_; ++i){
            values[i] = element->value_;
            element = element->prev_;
        }

        head_ = nullptr;
        for (ssize_t i = size_ - 1; i >= 0; --i){
            Push(values[i]);
        }

        delete[] values;
    }

    Stack() {
        head_ = nullptr;
        size_ = 0;
    }

    Stack(const Stack& other) : Stack() {
        Copy(other);
    }

    Stack& operator=(const Stack& other) {
        if (this == &other){
            return *this;
        }

        Copy(other);
        return *this;
    }

    ~Stack(){
        Clear();
    }
};

int RecogniseNumber(char s) {
    return s - '0';
}

int main() {
    Stack Numbers;
    char symb;
    while (std::cin >> symb) {
        if (isdigit(symb)) {
            int value = RecogniseNumber(symb);
            Numbers.Push(value);
        } else if (symb != '\n') {
            int first = Numbers.Top();
            Numbers.Pop();
            int second = Numbers.Top();
            Numbers.Pop();
            int result;
            if (symb == '+') {
                result = second + first;
            } else if (symb == '*') {
                result = second * first;
            } else {
                result = second - first;
            }

            Numbers.Push(result);
        }
    }

    std::cout << Numbers.Top() << '\n';

    return 0;
}
