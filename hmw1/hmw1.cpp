// Copyright 2021 Maciej Wozniak

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>

// It is ok to use the following namespace delarations in C++ source
// files only. They must never be used in header files.
using namespace std;
using namespace std::string_literals;
using StringVector = std::vector<std::string>;
using KeysValuesPair = std::pair <StringVector,std::unordered_map <std::string, std::vector<string>>>;

StringVector splitline(std::string &temp) { 
    // saving keys to vector
    // std::ifstream filename(f);
    // std::string temp;
    StringVector keyvector;
    std::string key;
    // split string into words 
    for (char &letter : temp) {
        if (letter == '"'){
            continue;
        }
        if (letter == '\t') {
            // if tab, the key is out and we have 
            keyvector.push_back(key);
            key = "";
        } else {
            key+=letter;
        }
    }
    keyvector.push_back(key);
    // filename.close();
    return keyvector;
}

// finding if a particula parameter was given 
bool findparam(StringVector &arguments, std::string &param) {
    return (std::find(arguments.begin(), arguments.end(), param) !=
            arguments.end());
}

// returns map with additional parameters filter and cols or colnums
std::unordered_map <std::string, StringVector> Modemap(StringVector &arguments) {
    std::unordered_map <std::string, StringVector> modemap;
    std::string key;
    for (auto a : arguments) {
        if (a[0] == '-') {
            key = a;
            modemap[a];
        }
        if (modemap.size() != 0 &&  a[0] != '-') {
            modemap[key].push_back(a);
        }   
    }
    return modemap;   
} 

// returning names of the columns which we have to print, in the order of printing
StringVector WhichColumns(StringVector &arguments, StringVector &keys, std::unordered_map <std::string, StringVector> &modemap) {       
    std::vector <std::string> cols2show;  // checking in which 'mode' we got the columns
    std::string cols = "--cols";
    std::string coln = "--colnums";
    if (findparam(arguments, cols)) { 
        cols2show = modemap[cols];
    } else if (findparam(arguments, coln)) {
        for (auto i:modemap[coln]) {  // take a key of index i (i is a string so we need to conv)
            cols2show.push_back(keys[std::stoi(i)]);
        }
    }
    return cols2show;
}

// reading the file, returning keys (list of col names) and values (all file in the map)
KeysValuesPair keysAndvalues(std::string filename) {
    std::ifstream file(filename);
    // read the first line with keys
    
    std::string temp;
    std::getline(file, temp, '\n');
    StringVector keys = splitline(temp);

    // now we have col names, let's put the key map here
    // question?? vector of vectors or map with keys as col and values

    std::unordered_map <std::string, std::vector<string>> values;
    for (std::string key : keys) {
        values[key];
    }

    // loop over the lines 
    // split lines
     // put the vals into the unordered map 
    while (std::getline(file, temp, '\n')) {
        StringVector vls = splitline(temp);
        for (int i = 0; i < vls.size(); i++) {
            values[keys[i]].push_back(vls[i]);
        }
    }
    return {keys,values};
}


std::vector<int> Indexes(KeysValuesPair &keysandvalues,StringVector &arguments,std::unordered_map <std::string, StringVector> &modemap,
StringVector &columns){
    int nofRows = keysandvalues.second[keysandvalues.first[0]].size();
    std::string flt = "--filter";
    if (!findparam(arguments, flt)){ 
        std::vector<int> indexes (nofRows);
        std::iota(std::begin(indexes), std::end(indexes), 0);
        return indexes;

    } else{
        std::vector<int> indexes;
        for (int i = 0; i < nofRows; i++) {
            if (keysandvalues.second[columns[0]][i].find(modemap[flt][0])!= std::string::npos){
                indexes.push_back(i);
            }
        }
        return indexes;
    }
}

void visualize(){

}

int main(int argc, char *argv[]) {
     // Check to ensure we have a data file specified.
    if (argc < 2) {
        std::cout << "Specify data file as command-line argument.\n";
        return 1;
    }
    // Open the data file and ensure it is readable.
    std::ifstream file(argv[1]);

       if (!file.good()) {  // check if file was open successfully
        std::cerr << "unable to read the file\n";
        return 1;
    }
    KeysValuesPair keysandvalues = keysAndvalues(argv[1]);
    // checking in which ''mode'' to print 
    StringVector arguments(argv + 1, argv + argc); 
    // getting mode map 
    std::unordered_map <std::string, StringVector> modemap = Modemap(arguments);
    // now, we have columns to print now just check if filter exist 
    StringVector columns = WhichColumns(arguments, keysandvalues.first,modemap);
    // indexes 
    std::vector<int> indexes = Indexes(keysandvalues,arguments,modemap,columns);
    // for (std::string col:columns) {
    //         if (col != columns.back()){
    //             std::cout << col << '\t';
    //         } else {
    //             std::cout << col;
    //         }
            
    //     }
    // std::cout << '\n';
    // // for (int i:indexes) {
    //     // for (std::string col:columns) {
    //     //     if (col != columns.back()){
    //     //         std::cout << keysandvalues.second[col][i] << '\t';
    //     //     } else {
    //     //         std::cout << keysandvalues.second[col][i];
    //     //     }
    //     // }
    //     std::cout << i;
    //     std::cout << '\n';
    // }


    return 0;
}






// End of source code
