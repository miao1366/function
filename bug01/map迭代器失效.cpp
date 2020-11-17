#include <iostream>
#include <map>
using namespace std;

typedef map<int, int> Map;
typedef map<int, int>::iterator MapIt;

void print(Map &m) {
    MapIt it;
    for(it = m.begin(); it != m.end(); it++) {
        cout << it->second << " ";
    }

    cout << endl;
}

void deleteValueFromMap(Map &m, int n = 5) {
    MapIt it;
    for(it = m.begin(); it != m.end(); /*不能再自增了*/) {
        if(0 == it->second % n)
        {
            m.erase(it++);
        }
        else
        {
            it++;
        }
    }
}

int main() {
    Map m;
    int i = 0;
    for(i = 0; i < 22; i++) {
        m[i] = i;
    }
    print(m);
    deleteValueFromMap(m); // 程序ok
    print(m);
    return 0;
}
