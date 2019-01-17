#ifndef TST_HPP
#define TST_HPP

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <cstring>
#include <vector>
#include "TSTNode.hpp"
using namespace std;

class Word {
  
  public:
    
    string word;
    int freq;

    Word(const string word, int freq) : word(word), freq(freq) {}

}; 

bool static wordCompare(const Word &p1,const Word &p2) { return p1.freq > p2.freq; }
bool static wordSort(const Word &p1,const Word &p2) { return p1.word.compare(p2.word)<0; }

class TST {

  public:
    /** Pointer to the root of this TST, or 0 if the TST is empty */
    TSTNode *root;

    /** Number of words stored in this TST. */
    unsigned int isize;

    /** Default constructor. Initialize an empty TST. */
    TST() : root(0), isize(0) {}

    /** Default destructor. Delete every node in this BST. */
    ~TST(){
	if(root)
	    deleteAll(root);
    }
    
    /** Build function to insert all the strings from corpus to the tree*/
    void build(const vector<string> & corpus) {

	unsigned int i = 0; // Index

	if(corpus.empty()) return;
	if(corpus.begin() == corpus.end()) return;

	// Check for root node
	if(!root) {
	    // Set root
            string word = corpus[i];
	    i++;
	    root = new TSTNode(word[0]);
	    // Set left letters
	    TSTNode *curr = root;
	    for(int i = 1; word[i]; i++) {
	        curr->mid = new TSTNode(word[i]);
	        curr = curr->mid;
	    }
	    curr->isWord = 1;
	    isize++;
        }

	// Insert every words
	for(; i < corpus.size(); i++) { 
	    // Set var
	    string word = corpus[i];
	    TSTNode *curr = root;
            // Insert the letters
            for(int ch = 0; word[ch]; ch++) {
	        // If not the first letter of the word
                if(ch != 0 && !curr->mid) {
	            curr->mid = new TSTNode(word[ch]);
		    curr = curr->mid;
		    continue;
                } else if (ch != 0 && curr->mid) {
		    curr = curr->mid;
	        }
                // not exist
	        while(curr) {
	            // Exist move to next node and letter
	            if((int)word[ch] == (int)(curr->let)) break;
	            // Non-exist, cond: check left
	            else if((int)word[ch] < (int)(curr->let)) {
	    	        if(curr->left) 
		            curr = curr->left;
		        else {
		            curr->left = new TSTNode(word[ch]);
		            curr = curr->left;
		            break;
	                }
	            }
	            // Non-exist, cond: check left
	            else if((int)word[ch] > (int)(curr->let)) {
	       	        if(curr->right) 
		            curr = curr->right;
		        else {
		            curr->right = new TSTNode(word[ch]);
		            curr = curr->right;
	    	            break;
	                }
	            }
	        }
	    }
	    // Set isWord and frequency
            curr->isWord = 1;
	    curr->freq++;
	    isize++;
        } // End insert all word 

    } // End of build function

    /** Return 10 Most frequent words based on the key word */
    vector<string> mostFreq(const string & key) const {

	vector<string> temp;
	TSTNode* curr = root;

	bool next = 0;
	for(char c:key){
	    while(1){
		if(next){ 
		    curr = curr->mid; 
	    	    next = 0;
		} 
		if(!curr)
		    return temp;
		if(curr->let == c){
		    next = 1;
		    break;
		}
		if(curr->let > c)
		    curr = curr->left;
		else
  		    curr = curr->right;
	    }
	}

	vector<Word> box;
	if (curr->isWord) box.push_back(Word(key,curr->freq));
	curr = curr->mid;
	setVector(&box,key,curr);
	sort(box.begin(),box.end(),wordCompare);
/*	for(unsigned int i = 0; i < 10; i++){
	    if(i<box.size())
		temp.push_back(box[i].word);
 	}*/
	vector<Word> sameFreq;
	for(unsigned int i = 0; i < 10; i++){
	    if(i<box.size()){
		sameFreq.push_back(box[i]);
		while((i+1)<box.size() && box[i+1].freq==box[i].freq){
		    sameFreq.push_back(box[i+1]);
		    i++;
		}
		sort(sameFreq.begin(),sameFreq.end(),wordSort);
		for(unsigned int j = 0; j<sameFreq.size();j++){
		    if(temp.size()<10)
		      	temp.push_back(sameFreq[j].word);
		}
		sameFreq.clear();
	    }
 	}
	return temp;

    }

    /** Return the number of words currently in the TST. */
    unsigned int size() const { return isize; }

    /** Return true if the TST is empty, else false. */
    bool empty() const { return isize==0;}

  private:
    /** Recursive function for searching */
    void setVector(vector<Word>* box,const string & word, TSTNode* curr)const {

	char result[BUFSIZ];
	strcpy(result,word.c_str());
	if(!curr) return;
	if(curr->left)
	    setVector(box,result,curr->left);
	if(curr->right)
	    setVector(box,result,curr->right);
        memset(result, 0, BUFSIZ);
	strcpy(result,word.c_str());
	result[word.length()] = curr->let;
	if(curr->isWord){
	    box->push_back(Word(result,curr->freq));
   	}
	if(curr->mid)
	    setVector(box,result,curr->mid);
    }

    void deleteAll(TSTNode* curr){
	
	if(curr->left)
	    deleteAll(curr->left);
	if(curr->right)
	    deleteAll(curr->right);
	if(curr->mid)
	    deleteAll(curr->mid);
	delete curr;
    }
    
};

#endif  // TST_HPP



