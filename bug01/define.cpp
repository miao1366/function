#include<iostream>  
using namespace std;

# define BAIDU_CONCAT(a, b) BAIDU_CONCAT_HELPER(a, b)
# define BAIDU_CONCAT_HELPER(a, b)  #a#b    //  
# define BAIDU_CONCAT_HELPER1(a, b) #a b
# define BAIDU_CONCAT_HELPER2(a, b) a##b
# define BAIDU_CONCAT_HELPER3(a, b) a ## b

int main(void) {
   
   cout<< BAIDU_CONCAT(aa,bb) << endl;      //  "aabb"
   //cout<< BAIDU_CONCAT1(aa,bb) << endl;     //  
   //cout<< BAIDU_CONCAT2(aa,bb) << endl;
}