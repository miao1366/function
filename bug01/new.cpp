#include <iostream>
using namespace std;
#include <stdlib.h>

class A
{
private:
    int x, y, z;
public:
    A(int _x, int _y,int _z) :x(_x), y(_y), z(_z)
    {
        cout << "Constructor" << endl;
    }
    ~A()
    {
        cout << "Destructor" << endl;
    }
    //void * operator new(size_t size);
    //void operator delete(void *p);
    friend ostream& operator <<(ostream& os, A& obj);

};

//
void* operator new(size_t size)
{
    cout << "my new" << endl;
    return malloc(size);
}

void operator delete(void *p)
{
    cout << "my delete" << endl;
    free(p);
}
ostream& operator <<(ostream& os, A& obj)
{
    os << obj.x << ",";
    os << obj.y << ",";
    os << obj.z << "\n";
    return os;
}

int main()
{
    A *p = new A(1, 2, 3);
    if (p == NULL)
    {
        printf("my new is falure\n");
        return -1;
    }

    cout << *p;

    delete(p);
    p = NULL;

    int *p1 = new int(3);
    cout << *p1;
    delete(p1);
    p1 = NULL;

    getchar();
}