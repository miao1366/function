#include <iostream>
using namespace std;

class Base
{
public:
    Base()
    {
       cout<<"Base:: Base()"<<endl;
    }

    ~Base()
    {
        cout<<"Base::~Base()"<<endl;
    }
    virtual Base& operator=(const Base& b)
    {
        cout<<"Base::operator=(const Base& b)"<<endl;
        return *this;
    }

};

class Derived:public Base
{
public:
    Derived()
    {
       cout<<"Derived:: Derived()"<<endl;
    }

    ~Derived()
    {
        cout<<"Derived::~Derived()"<<endl;
    }
    virtual Derived& operator=(const Base& b)
    {
        cout<<"Derived::operator=(const Base& b)"<<endl;
        return *this;
    }

};


int main()
{
    Derived d1,d2;
    Base b;

    //Base& pBase = d1;
    //pBase.operator=(d2);
    d1 = d2;
    d1 = b;
    return 0;
}
