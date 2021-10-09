#include <iostream>
#include <cmath>
#include <string>

double area(int rad){
    const double pi = 3.14;
    return pi * pow(rad,2);
}

void doit(int* i, std::string* s){
    *s += "***";
    *i += 2;
}

int main(){
    // int radius = 2; 
    // std::cout << "if rad is " << radius << " area of the circle is " 
    //     << area(radius) << std::endl;
    
    // pointers
    std::string s = "star";
    int i = 10;
    doit(&i, &s); // we are expecting pointers 
    std::cout << i << " and "<< s;


    return 0;
}