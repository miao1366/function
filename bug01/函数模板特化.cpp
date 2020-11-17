#include<iostream>  
using namespace std;

#include <iostream>
 
using namespace std;
 
template <typename T>
void fun(T a)
{
    cout << "void fun(T1 a)" << endl;
}
 
template <typename T1, typename T2>
void fun(T1 a, T2 b)
{
    cout << "void fun(T1 a, T2 b)" << endl;
}
 
void fun(int a, float b)
{
    cout << "void fun(int a, float b)" << endl;
}
 
int  main()
{
    int a = 0;
    float b = 0.0;
    fun(a);   
    fun(a, b);	//普通函数void fun(int a, float b)已经能完美匹配，于是调用普通函数
    fun(b, a);	//这个调用，函数模板有更好的匹配，于是调用函数模板
    fun<>(a, b);	//限定只使用函数模板
    return 0;
}