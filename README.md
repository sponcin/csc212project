# CSC212 BTree Project 

Our code creates are BTree through hashing and inserting words from text files into the appropriate nodes. The code can be compiled using g++. The code should be run with the following command line arguments: ./programName, book.txt, name_of_output_file, minimum_num_of_children_each_node_can_have. 

Note: the final command line argument will be used to calculate the size of the data vector in each node. The size is given by the formula: (2*_minimum_#_of_children_each_node_can_have_) - 1. So, an input of 4 will produce a data vector capable of containing 8 elements (0-7).
