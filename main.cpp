#include "btree.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

//declares function to calculate hash number
int hashFunction(std::string word);

//main function that takes in parameters - text file name, size of node vector and name of output file
int main(int argc, char*argv[])
{

    //initialize string called word to store each word in the file
    std::string word;

    //initialize integer variable to count the number of sentences
    int sentenceCount = 0;

    //initialize string variable to hold text file name
    std::string inputF = std::string(argv[1]);

    //
    std::ifstream input_file(inputF);

    //initialize int variable that stores the node vector size
    int minDeg = std::stoi(argv[2]);

    //initializes new BTree called myTree
    BTree myTree(minDeg);

    //get each word from input_file while a word is present
    while (input_file >> word) {

        //if any of the listed punctuation is at the end of a word, remove
        if (word[word.length() - 1] == ','|| word[word.length() - 1] ==':'|| word[word.length() - 1] =='"'|| word[word.length() - 1] ==';') {
            word.pop_back();
        }
        if (word[word.length() - 1] =='.'|| word[word.length() - 1] =='?'|| word[word.length() - 1] =='!') {
            word.pop_back();
            sentenceCount += 1;
        }

        //remove spaces
        if (word[0] == '"') {
            word.erase(0,1);
        }

        //transform all words to lowercase
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        //initialize integer key variable that calls hashFunction
        int key = hashFunction(word);

        //initialize boolean variable isPresent to check if the word is present in the tree
        bool isPresent;

        //calls previously initialize myTree to call the callSearch function
        myTree.callSearch(key,isPresent);

        //if word is not present, call the myTree insert function
        if (!isPresent) {
            myTree.insert(key,word);
        }
    }

    //closes the input file
    input_file.close();

    //calls the callTraverse function in myTree
    myTree.callTraverse();

    //calls the assign function in myTree
    myTree.assign();

    //uses mainline argument at index 3 for the filename of the output file
    std::string file_name = std::string(argv[3]);

    //calls the printdot function in myTree
    myTree.printdot(file_name);

    //cout the final sentence count
    std::cout << sentenceCount << std::endl;

    return 0;

}

//definition of hashFunction with word as the parameter
int hashFunction(std::string word) {
    //initialize key value to 0
    int key = 0;
    
    //for loop to go through the word
    for (int i = 0; i < word.length(); i++) {
        //initialize ascii integer variable that gets the int value of each character and multiplies
        //it by i + 1
        int ascii = int(word[i]) * (i + 1);
        //key is the ascii value plus the ascii of each previous letter of the word
        key += ascii;
    }

    //returns key to
    return key;
}