#include <iostream>
#include <string>
#include <algorithm>

bool isplaindrome(std::string &s) {
    std::string r = s;
    std::reverse(r.begin(), r.end());
    return s == r;
}
int main() {
     
    std::string s = "ssaass";
    std::cout << isplaindrome(s);
    return 0;

}