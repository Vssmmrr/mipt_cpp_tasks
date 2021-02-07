#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <queue>
#include <cstring>

template <class T>
class List {
    struct Node {
        Node* prev_ = nullptr;
        Node* next_ = nullptr;
        T value_;

        Node(Node* prev = nullptr, Node* next = nullptr, T value = 0): prev_(prev), next_(next), value_(value){
        }
    };

    Node* head_;
    Node* tail_;
    size_t size_;
public:
    using reference = T&;
    using value_type = T;
    using const_reference = const T&;
    using size_type = size_t;

    List() : head_(nullptr), tail_(nullptr), size_(0) {
    }

    ~List() {
        clear();
    }

    List(const List&) = delete;
    List& operator=(const List&) = delete;

    List(List<T>&& other) {
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;

        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    List& operator=(List<T>&& other) {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;

        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
        return *this;
    }

    void clear() {
        while (size_ > 0) {
            pop_front();
        }
    }

    void push_back(const T& value) {
        Node* newPrev = new Node(tail_, nullptr, value);
        if (tail_ != nullptr) {
            tail_->next_ = newPrev;
            tail_ = newPrev;
        } else {
            tail_ = head_ = newPrev;
        }
        ++size_;
    }

    void pop_front() {
        Node* newTop = head_->next_;
        delete head_;
        if (newTop != nullptr) {
            newTop->prev_ = nullptr;
            head_ = newTop;
        } else {
            tail_ = head_ = nullptr;
        }
        --size_;
    }

    T& front() {
        return head_->value_;
    }

    const T& front() const {
        return head_->value_;
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }
};


int main() {
    std::queue<long long, List<long long>> queue;

    std::string command;
    std::cin >> command;

    while(command != "exit") {

        if (command == "size") {
            std::cout << queue.size() << "\n";
        }
        else if (command == "pop") {
            if (!queue.empty()) {
                std::cout << queue.front() << '\n';
                queue.pop();
            } else {
                std::cout << "error\n";
            }
        }
        else if (command == "push") {
            int arg;
            std::cin >> arg;
            queue.push(arg);
            std::cout << "ok\n";
        }
        else if (command == "front") {
            if (!queue.empty()) {
                std::cout << queue.front() << '\n';
            } else {
                std::cout << "error\n";
            }
        }
        else if (command == "clear") {
            queue = {};
            std::cout << "ok\n";
        }

        std::cin >> command;
    }

    std::cout << "bye\n";

    return 0;
}
