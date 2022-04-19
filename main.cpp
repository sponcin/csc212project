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

    char* end;
    
    int b = 10;

    long int m = strtol(argv[2], &end, b);

    BTree myTree(m);

    while (input_file >> word) {

        if (word[word.length() - 1] == ','||'.'||'?'||':'||'"'||'!'||';') {
            word.pop_back();
        }

        if (word[0] == '"') {
            word.erase(0,1);
        }

        int key = hashFunction(word);

        bool isPresent;

        myTree.callSearch(key, isPresent);

// this code is throwing seg fault 11
        if (!isPresent) {
            myTree.insert(key, word);
        }
    }

    input_file.close();

}


int hashFunction(std::string word) {
    int key = 0;

    for (int i = 0; i < word.length(); i++) {
        int ascii = int(word[i]) * (i + 1);
        key += ascii;
    }

    return key;
}