#include <iostream>
#include <fstream>

int main(){
    std::ifstream inFile ("numbers.txt"); 
    if (!inFile.good()){ // check if file was open successfully
        std::cerr << "unable to read the file\n";
        return 1;
    }
    int sum = 0, num = 0;
        while (inFile >> num){ // until there are numbers in the file
            sum += num;
        }
    std::cout << "sum: " << sum << std::endl;
    return 0;
}