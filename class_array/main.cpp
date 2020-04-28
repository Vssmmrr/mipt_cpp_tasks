#include "class_array.h"
#include <iostream>

int main() {
    Array<int, 5> ar1;
    std::cout << ar1.Size() << '\n';
    ar1.Fill(5);
    for (size_t i = 0; i < ar1.Size(); ++i) {
        std::cout << ar1[i] << " ";
    }
    std::cout << "\n";
    ar1.Front() = 7;
    ar1.Back() = 11;
    std::cout << ar1.Front() << "   " << ar1.Back() << '\n';
    Array<int, 5> ar2;
    ar2.Fill(8);
    Get<4>(ar2) = 3;
    std::cout << Get<4>(ar2) << '\n';
    ar1.Swap(ar2);
    std::cout << (*ar1.data()) << "\n";
    std::cout << ar1.Empty() << '\n';
    std::cout << (ar1 > ar2) << '\n';
    std::cout << (ar1 == ar2) << '\n';
    std::cout << (ar1 != ar2) << '\n';
    std::cout << (ar1 < ar2) << '\n';
    std::cout << (ar1 >= ar2) << '\n';
    std::cout << (ar1 <= ar2) << '\n';
    return 0;
}
