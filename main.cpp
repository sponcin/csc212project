#include "btree.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <algorithm>

//declare hashfunction to create a unique number for each word
int hashFunction(std::string word);

//main function that takes in three parameters
int main(int argc, char*argv[])
{
    //gets length of the mainline argument
    //int len = sizeof(argv)/sizeof(argv[0]);

    //string that will be updated with each word in the file
    std::string word;

    //name of input file
    std::string inputF = std::string(argv[1]);

    //name of output file
    std::string outputF = std::string(argv[2]);

    //create input stream for while loop
    std::ifstream input_file(inputF);

    //initializes key variable
    int key;

    //initialize integer variable to count the number of sentences
    int sentenceCount = 0;

    //mimimum degree, see btree.h for explanation
    int minDeg = std::stoi(argv[3]);

    //initialize the btree using minimum degree
    BTree myTree(minDeg);

    //while the input stream receives words
    while (input_file >> word) {

         //if the current word ends with any of the following punctuation, increment sentenceCount
         if (word[word.length() - 1] =='.'|| word[word.length() - 1] =='?'|| word[word.length() - 1] =='!' || word[word.length() - 1] == '"') {
            sentenceCount += 1;
        }

        //loop through word, eliminate all punctuation except for apostrophes
        for (int i = word.size(); i >= 0; i--) {
            if (std::ispunct(word[i]) || word[i] == '"') {
                word.erase(i,1);
            }
        }


        //transform all words to lowercase
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        //hash the word
        key = hashFunction(word);

        //boolean used to determine if the word is present in the tree or not
        bool isPresent;

        //search what already exists in the tree. if present, count is updated
        myTree.callSearch(key,isPresent);

        //if the word is not present in the tree, insert it
        if (!isPresent) {
            myTree.insert(key,word);
        }
       
    }

    //close the file
    input_file.close();

    //traverse and print the contents of each node to the terminal
    myTree.callTraverse();

    //assign each node a number for the dot file
    myTree.assign();

    //create output file for dot format representation
    myTree.printdot(outputF);

    //output number of sentences in the text file
    std::cout << "This text file has " << sentenceCount << " sentences." << std::endl;

    //call the callPopular function for most popular word
    myTree.callPopular();

    //call the callUnique function to find the number of words excluding repeat counts
    myTree.callUnique();

    //call the findAvg function to find the average word length
    myTree.findAvg();

    std::cout << std::endl;

    //prompts the user to search for a word
    std::cout << "What word would you like to search for?" << std::endl;

    //initializes string variable searchWord
    std::string searchWord;

    //user now has to input the word to be searched
    std::cin >> searchWord;

    //we get the key value for this word
    key = hashFunction(searchWord);

    //calls the function to search the tree for the word and return it
    myTree.callTraverseSearch(key);

    return 0;

}


int hashFunction(std::string word) {
    //get hash value of each word
    std::hash<std::string> hash;
    int key = hash(word);

    //return sum
    return key;
}