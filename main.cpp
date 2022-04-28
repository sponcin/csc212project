#include "btree.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <algorithm>

int hashFunction(std::string word);

//main function that takes in three parameters
int main(int argc, char*argv[])
{

    //string that will be updated with each word in the file
    std::string word;

    //name of input file
    std::string inputF = std::string(argv[1]);

    //name of output file
    std::string outputF = std::string(argv[2]);

    //create input stream for while loop
    std::ifstream input_file(inputF);

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
        int key = hashFunction(word);

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
    //myTree.callTraverse();

    //assign each node a number for the dot file
    myTree.assign();

    //create output file for dot format representation
    myTree.printdot(outputF);

    //output number of sentences in the text file
    std::cout << "This text file has " << sentenceCount << " sentences." << std::endl;

    myTree.callPopular();

    myTree.findAvg();

    std::cout << "Would you like to search for a word in this file? Answer Yes or No" << std::endl;

    std::string response;

    std::cin >> response;

    if (response == "Yes") {
        //prompt user for word
        std::cout << "Please type the word you want to find. Type 0 to stop searching." << std::endl;

        std::string userSearch;

        //do search
        while (std::cin >> userSearch) {

            if (userSearch == "0") {
                break;
            }

            //stip word of punctuation, change all uppercase letters to lower
            for (int i = userSearch.size(); i >= 0; i--) {
                if (std::ispunct(userSearch[i]) || userSearch[i] == '"') {
                    userSearch.erase(i,1);
                }
            }

            //transform all words to lowercase
            transform(word.begin(), word.end(), word.begin(), ::tolower);

            //generate key
            int userKey = hashFunction(userSearch);

            int myCount = 0;
            bool isWordPresent;

            myTree.callUserSearch(userKey,isWordPresent,myCount);

            if (isWordPresent) {
                std::cout << "The word " << userSearch << " is in this text. It appears " << myCount << " times." << std::endl;
            } else {
                std::cout <<  "The word " << userSearch << " is not in this text." << std::endl;
            }
        }

    } else {
        std::cout << "Program end." << std::endl;
        return 0;
    }

    std::cout << "Program end." << std::endl;
    return 0;

}


int hashFunction(std::string word) {
    //get hash value of each word
    std::hash<std::string> hash;
    int key = hash(word);

    //return key
    return key;
}
