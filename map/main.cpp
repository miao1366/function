#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

    bool uniqueOccurrences() {
        map<int, int> mapNum;
        for (size_t i = 0; i < 10; i++) {
            mapNum[i+i] = mapNum[i+i] + 1;
        }
        map<int, int>::iterator it = mapNum.begin();
        while(it != mapNum.end()) {
            std::cout<<"iterator  " << it->first << "  " << it->second <<std::endl;
            it++;
        }
    }

    int main() {
        uniqueOccurrences();
    }