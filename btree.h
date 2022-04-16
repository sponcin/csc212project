#pragma once
#include <string>
#include <vector>

class BTNode
{
      private:
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

        int currentKeys;

        //vector of type wordData
        std::vector<wordData> data;

        //boolean to determine if the node is a leaf node
        bool isLeaf;


    public:
        BTNode();
        BTNode(std::string word, int key);

        ~BTNode();

    friend class BTree; //can access RBTNode in the RBTree class

};

class BTree
{
    private:
        BTNode* root;

        int minDeg;

    public:
        BTree(int minDeg_);
        ~BTree();

        void insert(int theKey, std::string theWord);
        void splitChild(int index, BTNode *parent, BTNode *toSplit);
        void insertNonFull(BTNode *newRoot, int theKey, std::string theWord);
        bool search(int theKey);

};