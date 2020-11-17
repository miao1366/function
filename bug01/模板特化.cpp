#include<iostream>  
using namespace std;



template<typename T1, typename T2>  
class Test  
{  
public:  
    Test(T1 i,T2 j):a(i),b(j){cout<<"模板类"<<endl;}  
private:  
    T1 a;  
    T2 b;  
};  
  
template<>  
class Test<int , char>  
{  
public:  
    Test(int i, char j):a(i),b(j){cout<<"全特化"<<endl;}  
private:  
    int a;  
    char b;  
};  
  
template <typename T2>  
class Test<char, T2>  
{  
public:  
    Test(char i, T2 j):a(i),b(j){cout<<"偏特化"<<endl;}  
private:  
    char a;  
    T2 b;  
}; 

int main() {
    Test<double , double> t1(0.1,0.2);  
    Test<int , char> t2(1,'A');  
    Test<char, bool> t3('A',true); 
    return 0;
}