#include<iostream>  
using namespace std;



template <int error_code> class BaiduErrnoHelper {};


template <> class BaiduErrnoHelper<(int)(3)> {};
//template <> class BaiduErrnoHelper<(int)(3)> {}; 重定义了

int main(void) {
    // template <> class BaiduErrnoHelper<(int)(4)> {};  a template declaration cannot appear at block scope
}