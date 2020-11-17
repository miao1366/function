#include<iostream>  
using namespace std;  
   

// 首先先回忆一下常成员函数
// 声明：<类型标志符>函数名（参数表）const；
// 说明：
// （1）const是函数类型的一部分，在实现部分也要带该关键字。
// （2）const关键字可以用于对重载函数的区分。
// （3）常成员函数不能更新类的成员变量，也不能调用该类中没有用const修饰的成员函数，只能调用常成员函数。
// （4）非常量对象也可以调用常成员函数，但是如果有重载的非常成员函数则会调用非常成员函数


class Test  
{  
protected:  
    int x;  
public:  
    Test (int i):x(i) { }  
    void fun() const  
    {  
        cout << "fun() const called " << endl;  
    }  
    void fun()  
    {  
        cout << "fun() called " << endl;  
    }  
};  

int foo(const Test & a) {
    cout << "foo() const & called " << endl;  
}

int foo(Test & a) {
    cout << "foo()  & called " << endl;  
}
   
int main()  
{  
    Test t1 (10);  
    const Test t2 (20);  
    t1.fun();  
    t2.fun();
    foo(t2); 
    foo(t1); 
    return 0;  
}

// 这种不允许。
#include<iostream>  
using namespace std;  

void fun(const int i)  
{  
    cout << "fun(const int) called ";  
}  
void fun(int i)  
{  
    cout << "fun(int ) called " ;  
}  
int main()  
{  
    const int i = 10;  
    fun(i);  
    return 0;  
}