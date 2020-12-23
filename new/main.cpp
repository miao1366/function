#include <iostream>
#include <string>

using namespace std;

inline string* dont_pass0toDEFINE_string(char *stringspot,
                                           const char *value) {
  return new(stringspot) string(value);
}

char s[] = "hello";
static string *s11 = dont_pass0toDEFINE_string(s, "world");

int main(void) {
    
    // string *s11 = dont_pass0toDEFINE_string(s, "world");
    cout << s << endl;
    return 0;
} 