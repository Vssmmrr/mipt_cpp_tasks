#include "my_string.h"
#include <iostream>
#include <cstring>


int main() {
    char* s = "hello!";
    String s1 = s;
    std::cout << s1 << '\n';

    String s2(5, 'f');
    std::cout << s2 << '\n';

    String ss;
    std::cin >> ss;
    std::cout << ss << '\n';

    std::cout << (s2 += ss) << '\n';
    s2 += 'd';
    std::cout << s2 + ss << '\n';
    std::cout << "first block test\n";
    std::cout << (s2 > ss) << '\n';
    std::cout << (s2 < ss) << '\n';
    std::cout << (s2 == ss) << '\n';
    std::cout << (s2 != ss) << '\n';
    std::cout << (s2 <= ss) << '\n';
    std::cout << (s2 >= ss) << '\n';
    std::cout << "second block test\n";
    std::cout << s2.CStr() << '\n';
    std::cout << s2.Data() << '\n';
    std::cout << s2.Empty() << '\n';
    std::cout << s2.Size() << '\n';
    std::cout << s2.Length() << '\n';
    std::cout << s2.Capacity() << '\n';
    std::cout << "third block test\n";
    ss.Front() = '5';
    ss.Back() = 'y';
    std::cout << ss << '\n';
    std::cout << ss.Front() << '\n';
    std::cout << ss.Back() << '\n';
    std::cout << "fourth block test\n";
    s2.ShrinkToFit();
    std::cout << s2.Capacity() << '\n';
    s2.Reserve(25);
    std::cout << s2.Capacity() << '\n';
    s2.Resize(5);
    std::cout << s2 << '\n';
    s1.Resize(30, 'Q');
    std::cout << s1 << '\n';
    s1.PushBack('z');
    std::cout << s1 << '\n';

    return 0;
}
