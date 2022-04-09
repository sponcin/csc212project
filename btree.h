#pragma once
#include <string>
#include <vector>

class BTNode
{
      private:
        std::string word;
        int key; // nodes can contain at least (key - 1) keys and a maximum of ((key * 2) - 1) keys 
        BTNode* left;
        BTNode* right;
        int hash;
        int ascii;
        int count;


    public:
        BTNode();
        BTNode(std::string word, int key);
        int asciiFunction(std::string word, int ascii);
        ~BTNode();

    friend class BTree; //can access RBTNode in the RBTree class



    // public:
    //     struct cell 
    //     {
    //         int row;
    //         int column;
    //         int val;
    //         bool visited;
    //     };
    //     int rows;
    //     int columns;
    //     int seed;
    //     std::string output_file_name;
    //     std::vector<std::vector<cell>> data;
    //     Maze(int seed, int rows, int columns, std::string output_file_name);
    //     void printMaze();
    //     void makeMaze();
    //     void writeMaze();

};

class BTree
{
    private:
        BTNode* root;

        BTNode* insert(int data, BTNode* root);
        int height(BTNode* root);

        bool search(int data, BTNode* root);

        BTNode* rotateLeft(BTNode* root);
        BTNode* rotateRight(BTNode* root);

    public:
        BTree();
        ~BTree();

        void insert(int data);
        int height();

        bool search(int data);

};