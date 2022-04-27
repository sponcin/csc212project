#pragma once
#include <string>
#include <vector>
#include <iostream>

class BTNode
{
      public:
        //child pointer
        std::vector<BTNode*> children;

        //struct containing the word, its resultant key, and a counter for duplicates
        struct wordData
        {
            std::string word;
            int key;
            int count = 0;
        };

        //minimum degree of the b tree
        //minimum degree:a fixed integer t >= 2. Every node other than the root must have at
        //least t-1 keys. Every internal node other than the root thus has at least t children.
        int minDeg;

        //current number of key, word pairs in the data vector
        int currentKeys;

        int id;

        //vector of type wordData
        std::vector<wordData> data;

        //boolean to determine if the node is a leaf node
        bool isLeaf;

        //constructor
        BTNode();
        //parameterized constructor
        BTNode(int minDegree_, bool leafy);

        //destructor
        ~BTNode();
};

class BTree
{

    public:
        //root pointer
        BTNode* root;

        //used to assign a numeric id to each node
        int idCounter = 0;

        //minimum degree
        int minDeg;

        //constructor
        BTree();
        //parameterized constructor
        BTree(int minDeg_);
        //destructor
        ~BTree();

        void insert(int theKey, std::string theWord);
        void splitChild(int index, BTNode *parent, BTNode *toSplit);
        void insertNonFull(BTNode *node, int theKey, std::string theWord);
        bool search(int key, BTNode* root);
        void searchWhenInput(int key, BTNode* root, bool &isPresent);
        void callSearch(int key, bool &isPresent);
        void printNode(BTNode* node);
        void traverse(BTNode* node);
        void callTraverse();
        void printdot(std::string file_name);
        void traverseDot(BTNode* node, std::ofstream &output_file);
        void assignID(BTNode* node);
        void assign();
        void mostPopular(BTNode* node, std::string &theWord, int &theCount);
        void callPopular();
        void avgLength(BTNode* node, float &sum, float &theCount);
        void findAvg();
        void uniqueWords(BTNode *node, int &uniqueCount);
        void callUnique();
};