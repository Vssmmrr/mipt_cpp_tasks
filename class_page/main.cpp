#include "page.h"
#include <iostream>

int main() {
    Page<int, 10> page; // создается страница с максимальной вместимостью 10
    page.PushBack(1);
    page.PushBack(2);
    page.PushBack(3); // page == [1, 2, 3]
    std::cout << page.Front() << '\n';
    std::cout << page.Back() << '\n';
    std::cout << page[1] << '\n';
    std::cout << page.Size() << '\n'; // == 3

    page.Clear(); // page.Size() == 0
    page.PushFront(4);
    page.PushFront(5);
    page.PushFront(6); // а теперь можно, page == [6, 5, 4]
    std::cout << page.Front() << "  6775" << '\n';
    std::cout << page.Back() << '\n';
    std::cout << page[1] << '\n';
    return 0;
}
