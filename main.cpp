#include "btree.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

//function declaration to read in the txt file
void ReadFile(std::string input_file_name, std::vector<std::string>* data);
int hashFunction(std::string word);

//main function that takes in two parameters
int main(int argc, char*argv[])
{
    //command line info at index 1 is the file name that the user is trying to open
    std::string input_file_name = std::string(argv[1]);

    //command line info at index 2 is the size of each child's vector
    std::string k = std::string(argv[2]);

    //initialize the string vector to hold read information from the txt file
    std::vector<std::string> data;

    //debugging/printing file name
    std::cout << input_file_name << std::endl;

    //debugging/printing k
    std::cout << k << std::endl;

    //function call to read the file and saves words to data vector
    ReadFile(input_file_name, &data);

    //gets size of data vector
    int size = data.size();

    //debugging/print size of data - currently keeps printing 0?
    std::cout << size << std::endl;

    //debugging/loops through and prints out data at all indices
    for(int i = 0; i < size; i++)
    {
        std::cout << data[i] << std::endl;
    }

}

//Read from a file
void ReadFile(std::string input_file_name, std::vector<std::string>* data){
    // Create the input filestream - opens the file & prepares it for reading
    std::ifstream file(input_file_name);

    // Creates a temporary vector to represent one row
    std::vector<std::string> new_row;

    // Temporary string to hold a single line of the file
    std::string str;

    // Reads all lines in the file, 1 at at time
    while (std::getline(file, str))
    {
      // Converts our string double to a stringstream
      std::istringstream ss(str);
      // Temp double to store a converted value from a line
      std::string token;

        // Reads all values from the stringstream (current row), converts to double
        while(ss >> token){
            // Adds the converted value to the row
            new_row.push_back(token);
        }
        new_row.clear();
    }
}


int hashFunction(std::string word) {
    int key = 0;

    for (int i = 0; i < word.length(); i++) {
        int ascii = int(word[i]) * i;
        key += ascii;
    }

    return key;
}