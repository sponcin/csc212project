#include "btree.h"
#include <fstream>
#include <iostream>
#include <iomanip>

BTNode::BTNode()
{
    //set minDeg to an arbitrary number
    minDeg = 2;

    //boolean set to true
    isLeaf = true;

    //sets resize data 
    data.resize((2*minDeg) - 1);

    //sets resize children
    children.resize(2*minDeg);

    //initialize ptrs to nullptr
    for (int i = 0; i < children.size(); i++) {
        children[i] = nullptr;
    }

    //currentKeys at 0
    currentKeys = 0;
}

BTNode::BTNode(int minDegree_, bool leafy)
{
    minDeg = minDegree_;

    isLeaf = leafy;

    data.resize((2*minDeg) - 1);

    children.resize(2*minDeg);

    //initialize ptrs to nullptr
    for (int i = 0; i < children.size(); i++) {
        children[i] = nullptr;
    }

    currentKeys = 0;
}


BTNode::~BTNode(){

}

BTree::BTree() {
    //set root to null
    root = nullptr;

    //minimum degree must be at least 2
    minDeg = 2;
}

BTree::BTree(int minDeg_) {
    //create an empty tree
    root = nullptr;

    //minimum degree provided by the user
    minDeg = minDeg_;
}

//destructor
BTree::~BTree() {

}

bool BTree::search(int key, BTNode* root) {
    //start at first index of the data vector in current node
    int index = 0;

    //while the index is still in bounds of the amt. of elements currently in the vector
    //and the key is greater than current element, increase index by 1
    while (index < root->currentKeys && key > root->data[index].key) {
        index++;
    }

    //if the index is in bounds and the key of the current element matches the key
    //we are searching for, we have found that this word does exist in the tree
    if (index < root->currentKeys && key == root->data[index].key) {
        //this word is in the tree
        std::cout << "The word you're searching for, " << root->data[index].word << "is in the tree and appeared " << root->data[index].count << " times." << std::endl;
        return true;
    }

    //if the above base case is not met and our current node is a leaf node,
    //we know that the word we searched for is not in the tree
    if(root->isLeaf == true) {
        //the word is not in the tree
        std::cout << "I'm sorry, " << root->data[index].word << " is not in the tree. Please try again." << std::endl;
        return false;
    }

    //if neither base case is hit, we recurse down a level, travelling to the proper child node
    return search(key, root->children[index]);
}

//calls the search function
void BTree::callUserSearch(int key) {
    search(key,root);
    return;
}

void BTree::searchWhenInput(int key, BTNode* root, bool &isPresent) {
    //if we have an empty tree, return immediately
    if (root == nullptr) {
        isPresent = false;
        return;
    }

    //start at first index of the data vector in current node
    int index = 0;

    //while the index is still in bounds of the amt. of elements currently in the vector
    //and the key is greater than current element, increase index by 1
    while (index < root->currentKeys && key > root->data[index].key) {
        index++;
    }

    //if the index is in bounds and the key of the current element matches the key
    //we are searching for, we have found that this word does exist in the tree
    if (index < root->currentKeys && key == root->data[index].key) {
        //this word is in the tree, boolean is now true so insert will not occur
        //add one to count
        isPresent = true;
        root->data[index].count++;
        return;
    }

    //if the above base case is not met and our current node is a leaf node,
    //we know that the word we searched for is not in the tree
    if(root->isLeaf == true) {
        //the word is not in the tree, boolean set to false so insert will occur
        isPresent = false;
        return;
    }

    //if neither base case is hit, we recurse down a level, travelling to the proper child node
    return searchWhenInput(key, root->children[index], isPresent);
}

//calls the searchWhenInput function
void BTree::callSearch(int key, bool &isPresent) {
    searchWhenInput(key,root,isPresent);
    return;
}

//invokes search algorithm
void BTree::insert(int theKey, std::string theWord) {
    //if we have an empty tree
    if (root == nullptr) {
        //call constructor for a new node, insert key and word into the first index of data vector.
        root = new BTNode(minDeg,true);
        root->data[0].key = theKey;
        root->data[0].word = theWord;
        root->data[0].count++;
        root->currentKeys++;
    } else {
        //if the node is full
        if (root->currentKeys == 2*minDeg - 1) {
            //create a new root
            BTNode* newRoot = new BTNode(minDeg,false);

            //the first child of the new root is the old root
            newRoot->children[0] = root;

            //split the child of the new root in two
            splitChild(0,newRoot,root);

            //since we know the new root is not full, we insert the key, word pair here.
            //recursion within this function will ensure it finds the right spot
            int i = 0;

            if (newRoot->data[0].key < theKey) {
                i++;
            }

            insertNonFull(newRoot->children[i], theKey, theWord);

            //change root
            root = newRoot;
        } else {
            //root is not full
            insertNonFull(root, theKey, theWord);
        }
    }
}

//function to split a child
void BTree::splitChild (int index, BTNode *parent, BTNode *toSplit) {
    //create a new node for the split
    BTNode *newChild = new BTNode(toSplit->minDeg,toSplit->isLeaf);

    //since this node is the result of splitting a full node, we know that it will contain one
    //less than the minimum degree keys
    newChild->currentKeys = minDeg - 1;

    //copy the right half of the data from full node
    for (int i = 0; i < minDeg - 1; i++) {
        newChild->data[i].key = toSplit->data[i + minDeg].key;
        newChild->data[i].word = toSplit->data[i + minDeg].word;
        newChild->data[i].count = toSplit->data[i + minDeg].count;
    }

    //if the node we are splitting is not a leaf, copy the right half of the child pointers vector
    if (toSplit->isLeaf == false) {
        for (int j = 0; j < minDeg; j++) {
            newChild->children[j] = toSplit->children[j + minDeg];
        }
    }

    //update the amount of keys in the node being split
    toSplit->currentKeys = minDeg - 1;

    //shift over pointers to make space for the new child
    for (int j = parent->currentKeys; j >= index + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }

    //place new child node into children vector of the parent
    parent->children[index + 1] = newChild;

    //move keys over to make space for key coming from toSplit
    for (int j = parent->currentKeys - 1; j >= index; j--) {
        parent->data[j + 1].word = parent->data[j].word;
        parent->data[j + 1].key = parent->data[j].key;
        parent->data[j + 1].count = parent->data[j].count;
    }

    //copy middle element of data vector from node being split to parent
    parent->data[index].word = toSplit->data[minDeg - 1].word;
    parent->data[index].key = toSplit->data[minDeg - 1].key;
    parent->data[index].count = toSplit->data[minDeg - 1].count;

    //update number of keys in parent vector
    parent->currentKeys++;

}

void BTree::insertNonFull(BTNode *node, int theKey, std::string theWord) {
    //starting from rightmost end of data vector
    int i = node->currentKeys - 1;

    if (node->isLeaf) {
        //move each element right by one until the point where key should go
        //while hash greater than current hash, keep looking. insert when right position is found.
        while (i >= 0 && theKey < node->data[i].key) {
            node->data[i + 1].key = node->data[i].key;
            node->data[i + 1].word = node->data[i].word;
            node->data[i + 1].count = node->data[i].count;
            i--;
        }

        //place key, word pair in the data vector
        node->data[i + 1].key = theKey;
        node->data[i + 1].word = theWord;
        node->data[i + 1].count = 1;

        //increase the current number of keys
        node->currentKeys++;

        //return out
        return;

    } else {

        //use i to locate child where key, word pair belongs
        while (i >= 0 && theKey < node->data[i].key) {
            i--;
        }

        //since we know the proper place for the key, word pair is in the ith
        //child node of our current node, the key must be less than or equal to
        //the key at the current index. add 1 to i to find the right index in the children vector
        i++;

        //if the child node where our key, word pair should go is full, we must split it
        if (node->children[i]->currentKeys == ((2*minDeg) - 1)) {
            splitChild(i, node, node->children[i]);

            //splitting the child may have changed the correct position of i, so we update it
            if (theKey > node->data[i + 1].key) {
                i++;
            }
        }
        //recurse to the correct location using new index
        insertNonFull(node->children[i], theKey, theWord);
    }
}

//prints full node to terminal
void BTree::printNode(BTNode* node) {
    //for each element in the node, print the key, word, and count
    for (int i = 0; i < node->currentKeys; i++) {
        std::cout << node->data[i].key << " ";
        std::cout << node->data[i].word << " ";
        std::cout << node->data[i].count << " ";
        std::cout << std::endl;
    }
    return;
}

//traverses through tree and calls printNode to print whole tree
void BTree::traverse(BTNode* node) {
    //print the contents of the node
    printNode(node);
    std::cout << "--------" << std::endl;

    //loop of recursive calls provided the node is not a leaf
    for (int i = 0; i <= node->currentKeys; i++) {
        if(!node->isLeaf) {
            traverse(node->children[i]);
        }
    }
}

//public facing function that traverses the tree
void BTree::callTraverse() {
    traverse(root);
    return;
}

void BTree::assignID(BTNode* node) {
    //every time a node is visited, it's id is one more than the previous node
    node->id = idCounter++;

    //loop of recursive calls to the children of the current node
    for (int i = 0; i <= node->currentKeys; i++) {
        if(!node->isLeaf) {
            assignID(node->children[i]);
        }
    }
}

//public facing assignID function
void BTree::assign() {
    assignID(root);
    return;
}

//function to open an output file to start the dot file
void BTree::printdot(std::string file_name)
{
    //Output file given name of file
    std::ofstream output_file(file_name);
    output_file << "graph graphname {" << std::endl; //preparing file before handing it to recursdot
    traverseDot(root, output_file);
    output_file << "}" <<std::endl; //end of output_file
}

//function to traverse the tree and print the dot file
void BTree::traverseDot(BTNode* node, std::ofstream &output_file) {

    //assigns label to node
    output_file << node->id << " [label =  \"";
    //loop through current node, retrieve the words and count within the node
    for(int i = 0; i < node->currentKeys; i++)
    {
        //write to output
        output_file << node->data[i].word << " " << node->data[i].count <<  " | ";
    }

    //ends label
    output_file << "\"];" << std::endl;

    //loop through children vector
    for(int i = 0; i <= node->currentKeys && node->children[i] != nullptr; i++)
    {
        //prints links between current node and its children
        output_file << node->id << " -- " << node->children[i]->id << std::endl;

    }

    //if not a leaf node, recursively call to each child of current node
    for (int i = 0; i <= node->currentKeys; i++) {
        if(!node->isLeaf) {
            traverseDot(node->children[i], output_file);
        }
    }
}

//finds most popular word
void BTree::mostPopular(BTNode* node, std::string &theWord, int &theCount) {

    for (int i = 0; i < node->currentKeys; i++) {
        //if current element has a greater count than our current count
        if (node->data[i].count > theCount) {
            //count is updated to count of current element, current word becomes the word
            theCount = node->data[i].count;
            theWord = node->data[i].word;
        }
    }

    //loop of recursive calls provided the node is not a leaf
    for (int i = 0; i <= node->currentKeys; i++) {
        if(!node->isLeaf) {
            mostPopular(node->children[i],theWord,theCount);
        }
    }
}

//public facing function to find the most popular word
void BTree::callPopular() {
    //string and int vars that will be updateed in call of mostPopular
    std::string theWord;
    int theCount = 0;

    mostPopular(root,theWord,theCount);

    std::cout << "The most popular word in this text file is '" << theWord << "', which appears " << theCount << " times." << std::endl;
    return;
}

//function to find the average length of words in the file
void BTree::avgLength(BTNode *node, float &sum, float &numElements) {
    //for each element in the current node
    for (int i = 0; i < node->currentKeys; i++) {

        //find the length of the current word
        std::string theWord = node->data[i].word;

        //multiply length of word by the number of times it appears in the text
        int length = theWord.length() * node->data[i].count;

        //add this value to the sum
        sum += length;

        //add the count of curr element to total number of elements/words
        numElements += node->data[i].count;
    }

    //loop of recursive calls provided the node is not a leaf
    for (int i = 0; i <= node->currentKeys; i++) {
        if(!node->isLeaf) {
            avgLength(node->children[i],sum,numElements);
        }
    }
}

//public facing function to find the average word length
void BTree::findAvg() {
    //float values that are updated when avgLength is called
    float sum = 0;
    float numElements = 0;

    avgLength(root,sum,numElements);

    //take the average
    float avg = sum/numElements;


    std::cout << "This text file has " << numElements << " words in total." << std::endl;
    std::cout << "This text file has an average word length of " << std::setprecision(2) << avg << " letters." << std::endl;
    return;
}

//function to find the amount of unique words (word count without duplicates)
void BTree::uniqueWords(BTNode *node, int &uniqueCount) {
    //count each word in the node, disregarding duplicates
    for (int i = 0; i < node->currentKeys; i++) {
        uniqueCount++;
    }

    //loop of recursive calls provided the node is not a leaf
    for (int i = 0; i <= node->currentKeys; i++) {
        if(!node->isLeaf) {
            uniqueWords(node->children[i],uniqueCount);
        }
    }
}

//prints the unique amount of words
void BTree::callUnique() {
    //int var that gets updated via uniqueWords
    int uniqueCount = 0;

    uniqueWords(root,uniqueCount);

    std::cout << "The unique word frequency of this text file is " << uniqueCount << "." << std::endl;

    return;
}