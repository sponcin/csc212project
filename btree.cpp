#include "btree.h"

BTNode::BTNode()
{
    minDeg = 2;

    isLeaf = true;

    data.resize((2*minDeg) - 1);

    children.resize(2*minDeg);

    currentKeys = 0;
}

BTNode::BTNode(int minDegree_, bool leafy)
{
    minDeg = minDegree_;

    isLeaf = leafy;

    data.resize((2*minDeg) - 1);

    children.resize(2*minDeg);

    currentKeys = 0;
}


BTNode::~BTNode(){

}

BTree::BTree() {
    root = nullptr;

    minDeg = 2;
}

BTree::BTree(int minDeg_) {
    //create an empty tree
    root = nullptr;

    minDeg = minDeg_;
}

BTree::~BTree() {

}

bool BTree::search(int key, BTNode* root) {
    //start at first index of the data vector in current node
    int index = 0;

    //while the index is still in bounds of the amt. of elements currently in the vector
    //and the key is greater than current element, increase index by 1
    while (index < root->currentKeys - 1 && key > root->data[index].key) {
        index++;
    }

    //if the index is in bounds and the key of the current element matches the key
    //we are searching for, we have found that this word does exist in the tree
    if (index <= root->currentKeys - 1 && key == root->data[index].key) {
        //this word is in the tree
        return true;
    }

    //if the above base case is not met and our current node is a leaf node,
    //we know that the word we searched for is not in the tree
    if(root->isLeaf == true) {
        //the word is not in the tree
        return false;
    }

    //if neither base case is hit, we recurse down a level, travelling to the proprer child node
    return search(key, root->children[index + 1]);
}

void BTree::searchWhenInput(int key, BTNode* root, bool &isPresent) {
    //if we have an empty tree, return immediately
    if (root == nullptr) {
        isPresent == false;
        return;
    }

    //start at first index of the data vector in current node
    int index = 0;

    //while the index is still in bounds of the amt. of elements currently in the vector
    //and the key is greater than current element, increase index by 1
    while (index < root->currentKeys - 1 && key > root->data[index].key) {
        index++;
    }

    //if the index is in bounds and the key of the current element matches the key
    //we are searching for, we have found that this word does exist in the tree
    if (index <= root->currentKeys - 1 && key == root->data[index].key) {
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

    //if neither base case is hit, we recurse down a level, travelling to the proprer child node
    return searchWhenInput(key, root->children[index + 1], isPresent);
}

void BTree::callSearch(int key, bool &isPresent) {
    searchWhenInput(key,root,isPresent);
    return;
}


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
            insertNonFull(newRoot, theKey, theWord);
        } else {
            //root is not full
            insertNonFull(root, theKey, theWord);
        }
    }
}

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
            if (theKey > node->data[i].key) {
                i++;
            }
        }
        //recurse to the correct location using new index
        insertNonFull(node->children[i + 1], theKey, theWord);
    }
}