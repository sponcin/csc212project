#include "btree.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

int hashFunction(std::string word);

//main function that takes in two parameters
int main(int argc, char*argv[])
{

    std::string word;

    std::string inputF = std::string(argv[1]);

    std::ifstream input_file(inputF);

    int minDeg = std::stoi(argv[2]);

    BTree myTree(minDeg);

    while (input_file >> word) {

        if (word[word.length() - 1] == ','|| word[word.length() - 1] =='.'|| word[word.length() - 1] =='?'|| word[word.length() - 1] ==':'|| word[word.length() - 1] =='"'|| word[word.length() - 1] =='!'|| word[word.length() - 1] ==';') {
            word.pop_back();
        }

        if (word[0] == '"') {
            word.erase(0,1);
        }

        int key = hashFunction(word);

        bool isPresent;

        myTree.callSearch(key,isPresent);

        if (!isPresent) {
            myTree.insert(key,word);
        }
    }

    input_file.close();

    myTree.callTraverse();

    return 0;
}


int hashFunction(std::string word) {
    int key = 0;

    for (int i = 0; i < word.length(); i++) {
        int ascii = int(word[i]) * (i + 1);
        key += ascii;
    }

    return key;
}