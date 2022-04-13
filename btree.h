#pragma once
#include <string>
#include <vector>

class BTNode
{
      private:
        //child pointer
        BTNode* child;
        
    public:
        struct wordData 
        {
            std::string word;
            int hash;
            int ascii;
            int count;
        };
        //how many spaces available in a vector (minus 1)
        int key; 
        //vector using wordData data type
        std::vector<wordData> data; 
        BTNode();
        BTNode(std::string word, int key);
        int hashFunction(std::string word);

        ~BTNode();

    friend class BTree; //can access RBTNode in the RBTree class

};

class BTree
{
    private:
        BTNode* root;

        BTNode* insert(int data, BTNode* root);

        bool search(int data, BTNode* root);

        BTNode* splitChild(int data, BTNode* root);

    public:
        BTree();
        ~BTree();

        void insert(int data);

        bool search(int data);

        void splitChild(int data);

    //data_size = size of the vectors
    //children size = data_size + 1
};