#include <iostream>
#include <string>
#include <regex>
using namespace std;

//. 表示一个除了\n以外的任意一个字符。\.表示一个真正的.符号
//* 字符出现没有或更多
//+ 一次或更多
//？ 没有或一次 
// . 是匹配除过\n 之外的全部自符 用[\d\D] 匹配所有字符。或者是[\s\S]


int main() {
    string str("Serial          : 189f9d8445ba3c52\n");
    //string str("bog@gmail.com");
    //string pattern(".*[0-9a-fA-F]+[\\d\\D]*");
    string pattern("[0-9a-fA-F]+");
    //string pattern("([[:w:]]+)@([[:w:]]+)\\.com");
    smatch result;
    regex re(pattern);
    if (regex_search(str, result, re)) {
        for(size_t i = 0; i < result.size(); i++) {
            std::cout<< result.str(i) << std::endl;
        }
    } else {
        std::cout << "no match" <<endl;
    }
    return 0;
}

// int main() {
//     string str("Serial          : 189f9d8445ba3c52\n");
//     size_t pos =  str.find_last_of(' ');
//     string str1;
//     if (pos != string::npos) {
        
//         str1 = str.substr(pos+1);
        
//     }
//     cout<<str1<<endl;
//     return 0;
// }
