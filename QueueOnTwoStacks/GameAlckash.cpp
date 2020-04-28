#include <cstddef>
#include <iostream>
typedef int T;

struct Node {
    Node* prev_;
    T value_;

    Node(Node* prev, T value) : prev_(prev), value_(value){
    }
};

class Stack {
private:
    Node* head_;
    size_t size_;

    void Copy(const Stack& other) {
        this->Clear();
        //this->size_ = other.size_;

        T* values = new T[size_];
        Node* element = other.head_;
        for(size_t i = 0; i < other.size_; ++i) {
            values[i] = element->value_;
            element = element->prev_;
        }

        head_ = nullptr;
        for (ssize_t i = other.size_ - 1; i >= 0; --i) {
            Push(values[i]);
        }

        delete[] values;
    }

public:
    Stack() : head_(nullptr), size_(0) {
    }

    ~Stack() {
        Clear();
    }

    void Push(T value) {
        Node* newPrev = new Node(head_, value);
        head_ = newPrev;
        ++size_;
    }

    void Pop() {
        if (Empty()) {
            return;
        }
        Node* newTop = head_->prev_;
        delete head_;
        head_ = newTop;
        --size_;
    }

    T Top() const {
        return head_->value_;

    }

    T& Top() {
        return head_->value_;
    }

    bool Empty() const {
        return size_ == 0;
    }

    size_t Size() const {
        return size_;
    }

    void Clear() {
        while (!Empty()) {
            Pop();
        }
    }

    Stack& operator=(const Stack& other) {
        if (this == &other) {
            return *this;
        }

        Copy(other);
        return *this;
    }

    Stack(const Stack& other) : Stack() {
        Copy(other);
    }
};

class Queue{
private:
    //size_t size_;
    mutable Stack push_stack_;
    mutable Stack pop_stack_;

    void TransferValues() {
        while (!push_stack_.Empty()){
            pop_stack_.Push(push_stack_.Top());
            push_stack_.Pop();
        }
    }
public:
    void Push(T value) {
        push_stack_.Push(value);
    }

    void Pop() {
        if (Empty()) {
            return;
        }

        if (pop_stack_.Empty()) {
            TransferValues();
        }

        pop_stack_.Pop();
    }


    T& Front() {
        if (pop_stack_.Empty()) {
            TransferValues();
        }

        return pop_stack_.Top();
    }

    void Clear() {
        push_stack_.Clear();
        pop_stack_.Clear();
    }

    bool Empty() const {
        return push_stack_.Empty() && pop_stack_.Empty();
    }

    size_t Size() const {
        return (push_stack_.Size() + pop_stack_.Size());
    }


    Queue(const Queue& other) = default;
    Queue& operator=(const Queue& other) = default;
    Queue() = default;
    ~Queue() = default;
};

void GetCards(Queue& deck){
    const int initial_number_of_cards_ = 5;
    for (int i = 0; i < initial_number_of_cards_; ++i){
        int card;
        std::cin >> card;
        deck.Push(card);
    }
}

bool SecondWins(T& first,T& second){
    return (first == 9 && second == 0) ||
           (first < second &&
            !(first == 0 && second == 9));
}

void OneMove(Queue& first, Queue& second){
    if (SecondWins(first.Front(), second.Front())){
        second.Push(first.Front());
        second.Push(second.Front());
        first.Pop();
        second.Pop();
    } else {
        first.Push(first.Front());
        first.Push(second.Front());
        first.Pop();
        second.Pop();
    }
}

void Game(Queue& first_cards, Queue& second_cards, int& number_of_moves,
        const int& max_number_of_moves) {
    while (!first_cards.Empty() && !second_cards.Empty() &&
           number_of_moves < max_number_of_moves) {
        ++number_of_moves;
        OneMove(first_cards, second_cards);
    }
}

int main() {
    Queue first_cards;
    Queue second_cards;
    GetCards(first_cards);
    GetCards(second_cards);

    int number_of_moves = 0;
    const int max_number_of_moves = 1'000'000;

    Game(first_cards, second_cards, number_of_moves, max_number_of_moves);

    if (first_cards.Empty()) {
        std::cout << "second " << number_of_moves << '\n';
    } else if (second_cards.Empty()) {
        std::cout << "first " << number_of_moves << '\n';
    } else if (number_of_moves >= max_number_of_moves) {
        std::cout << "botva\n";
    }

    return 0;
}
