// "Copyright 2021 <Maciej Wozniak>"

#include <iostream>
#include <fstream>

int main(int arcg, char *argv[]) {
    std::ifstream inFile(argv[1]);

    int biggest;
    int secondbiggest;
    inFile >> biggest >> secondbiggest;
    if (biggest < secondbiggest) {
        std::swap(biggest, secondbiggest);
    }
    int num;
    while (inFile >> num) {  // until there are numbers in the file
        if (num > biggest) {
            secondbiggest = biggest;
            biggest = num;
        } else if (num < biggest && num > secondbiggest) {
            secondbiggest = num;
        }
    }
    std::cout << "2nd max = " << secondbiggest << std::endl;
    return 0;
}
