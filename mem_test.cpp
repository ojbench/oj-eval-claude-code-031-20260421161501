#include "src.hpp"
#include <iostream>
#include <vector>

void test_memory() {
    for (int i = 0; i < 100000; ++i) {
        pylist ls;
        ls.append(ls);
    }
}

int main() {
    test_memory();
    std::cout << "Memory test finished" << std::endl;
    return 0;
}
