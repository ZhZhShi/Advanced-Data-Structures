#ifndef TSTNODE_HPP
#define TSTNODE_HPP

#include <string>
using namespace std;

class TSTNode {

  public:

    TSTNode(const char let) : let(let) { 
	left = right = mid = 0; 
	isWord = false;
	freq = 0;
    }

    TSTNode *left;
    TSTNode *right;
    TSTNode *mid;
    // TSTNode *parent;
    char const let;  // the const Data in this node.
    bool isWord;
    int freq; // Frequence of the word 

};

#endif  // TSTNODE_HPP
