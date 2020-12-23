#include "./tesh.h"


// void print() {
//     cout << "inline print" << endl;
//     cout << "inline print" << endl;
// }

int main(void) {
    print(1);
    void (*func)(int) = print;
    func(1);
    return 0;
}