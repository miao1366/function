#include <iostream>
#include <string>
using namespace std;

class A {
public:
    template<typename T, class CB>
    static void func(const std::string &name, T *first, T *last, CB callback) {
        std::cout << "hello world" << std::endl;
        callback();
        return ;
    }
};

int main() {   
    int arr[10] = {0};
    A::func("hello", arr, arr + 10, [](void){
        std::cout << "hello world" << std::endl;
    });
    return 0;
}