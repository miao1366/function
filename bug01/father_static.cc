#include <iostream>

class Base{
public:
    static const int a;
};

class D1 : public Base {

};

class D2 : public Base {

};

const int Base::a = 2;

int main(void) {
    D1 d;
    //d.a = 3;
    std::cout << D2::a << std::endl;
}