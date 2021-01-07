#include <iostream>
#include <exception>

using namespace std;

int func() {
    try {
        throw 1;
    }

    catch(...) {
        std::cerr << " e.what() " << '\n';
    }
    std::cout << "catch later" << std::endl;
    return 0;
}

int main() {
    func();
    return 0;
}