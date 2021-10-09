#include <string>
#include <iostream>

bool isLucky(int n) {
   std::string nstr = std::to_string(n);
    int firsthalf = 0;
    for(int i = 0; i<nstr.length()/2;i++){
        int val = nstr[i] - 48;
        firsthalf+= val;
    }
    int secondhalf = 0;
    for(int i =nstr.length()/2 ; i<nstr.length();i++){
        int val = nstr[i] - 48;
        secondhalf+= val;
    }
    return firsthalf == secondhalf;
    
}