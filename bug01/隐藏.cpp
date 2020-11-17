#include <iostream>
#include <string>
using namespace std;

class base{
public:
    void foo(int a) {
        std::cout << "hello base" << endl;
    }

};

class dd: public base{
public:
    void foo(int a, int b) {
        std::cout << "hello dd" << endl;
    }

};

int main() {
    dd d;
    d.foo(2);
    return 0;
}