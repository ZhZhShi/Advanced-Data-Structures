/**
 * CSE 100 PA2 Document Generator
 */

#ifndef DOCUMENT_GENERATOR_HPP
#define DOCUMENT_GENERATOR_HPP

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unordered_map>

using namespace std;

class Word{
  public:
    string self;	
    int count;
    unordered_map<string,int> freq;
    vector<string> ratios;
    vector<double> ration;
    bool isSorted = 0;
    Word(){};
	
    Word(const string & a):self(a){};
	
    void add(const string & name){
	count++;
	unordered_map<string,int>::iterator it = freq.find(name);
	if(it==freq.end()){
	    freq.insert(make_pair(name, 1));
	}else{
	    freq[name]++;
	}
    }

    string find(double rand){
	if(!isSorted) 
	    goSort();
	double prev = 0;
	for(unsigned int i = 0; i < ratios.size(); i++){
	    if(rand < ration[i] && rand >= prev)
		return ratios[i];
	    prev=ration[i];
	}
	return "";
    }

    void goSort(){
	unordered_map<string, int> :: iterator it;
	int ccount = 0;
	for(it = freq.begin(); it != freq.end(); it++){
	    ratios.push_back(it->first);
	    ration.push_back(((double)(it->second+ccount))/count);
	    ccount += it->second;
	}
	isSorted=1;
    }
};


class DocumentGenerator {

public:
 
  unordered_map<string,Word> relation;

/**
 * Builds a document generator from the documents in the given directory
 * This should open all files in the directory, read them in, tokenize them into words,
 * and build the datastructure from that stream of words.
 *
 * To tokenize a document, you are required to do the following in this order 
 * (or in a manner that is logically equivalent to doing them in this order):
 *  - remove all UNWANTED_CHARACTERS from the document
 *  - split the document into different tokens based on whitespace
 *    (treat all whitespace breaks equally - newlines and spaces should be treated the same way. 
 *    Whitespace is thrown away and not included in tokens. There should be no empty tokens.)
 *  - additionally split each PUNCTUATION character into its own token
 *    (equivalently - treat all punctuation as if it was surrounded on both sides by a space)
 *  - characters that are neither UNWANTED_CHARACTERS nor PUNCTUATION should be treated normally, 
 *    the same way as any alphabetic character. This includes single apostrophes and accented 
 *    characters.
 *  - from here on, we'll use the word "word" to refer to all tokenized strings, such as "hello",
 *    "." or ","
 *
 * So, for instance, the short sentence
 * "I like the man's $10,000 trains. Sally jumped $ ov^er the moon, I think? I. I think."
 * Would be tokenized into ["I", "like", "the", "man's", "10", ",", "000", "trains", ".", "Sally",
 * "jumped", "over", "the", "moon", ",", "I", "think", "?", "I", ".", "I" "think", "."]
 * and the frequencies of the words after "I" would be;
 *   like - 1
 *   think - 2
 *   . - 1
 *
 * A few notes:
 * 1) you must treat words with different capitalizatoins differently (so "foo" and "Foo"
 *    are different words, and the frequencies of the words that follow (or precede)
 *    "foo" will be different than the frequencies of the words that follow (or precede) "Foo" )
 * 2) pretend that the first word in each document is preceeded by a periood (That way, it is
 *    considered when starting any new sentence)
 */
  DocumentGenerator(const string & documentsDirectory){
    vector<string>* names = getWords(documentsDirectory);
    relation.insert(make_pair(".", Word(".")));
    string parent;
    // Insert word
    vector<string>::iterator it = names->begin();
    for(; it != names->end()-1; it++){
        if(*it == " ") {
	    parent = ".";
	}
	else {
	    if(relation.find(*it) == relation.end())
		relation.insert(make_pair(*it, Word(*it)));
	    relation[parent].add(*it);
	    parent = *it;
	}
    }
    delete names;
  };

/**
 * Suppose you're in the middle of generating a document and that you've just added the word
 * prevWord to the document. 
 * generateNextWord(prevWord) should generate the next word at random for the document, 
 * according to the frequencies of all the words that followed prevWord in the input documents
 *
 * So, for instance, if in all the input documents the word "foo" was followed by the word "bar"
 * twice and "baz" 8 times,
 * then 80% of the time generateNextWord("foo") should return "baz" and 20% of the time you
 * should return "bar".
 * 
 * This method should return punctuation words like "." and "," as if they were any other word.
 * Likewise, it should be able to take punctuation words as input (generateNextWord(","))
 * 
 * You can assume that prevWord is always a word that's present in one of the documents you've
 * read in.
 */
  string generateNextWord(const string & prevWord){
    return relation[prevWord].find((double)rand()/(RAND_MAX));	
  };

/**
 * Generate a document with numWords words in it.
 *
 * See the docs on generateNextWord for a description of what it means to generate a word.
 * After generating all the words - concatenate them all into a single string representing
 * the entire document, and return that string.
 *
 * Notes:
 * The first word you generate should be as if the previous word was '.'
 * Your document should not contain whitespace except for spaces.
 * Your should insert a single space in front of each word except:
 *  - Don't insert spaces in front of punctuation marks
 *  - Don't insert a space at the start of the document
 * Punctuation "words" count against this word total.
 * If you generate a word which has no successor in the documents you
 *   are mimicing (because the word only appeared at the ends of documents)
 *   generate '.' as the next word.
 *
 * The document will likely not end at the end of a sentence. That's okay.
 */
  string generateDocument(const int numWords){
    string prefix = ".";
    string ret = "";
    srand(time(NULL));
    for(int i = 0; i < numWords; i++){
	prefix = generateNextWord(prefix);
	if(prefix == "")
	    prefix = ".";
	if(PUNCTUATION.find(prefix) == string::npos && i != 0)
	    ret += " ";
	ret += prefix;
    }
    return ret;	
  };

/**
 * Destructor
 */
  ~DocumentGenerator(){};

/**
 * Break a line over whitespace separators 
 * Caller must delete returned vector
 */
  vector<string> * tokenize(const string & line) {
    vector<string> * words = new vector<string>();
    vector<char> word;
    for (unsigned char c : line) {
	// Find punctuation chars
	if (PUNCTUATION.find(c) != string::npos) {
	    words->push_back(string(word.begin(), word.end()));
	    word.clear();
	    word.push_back(c);
	    words->push_back(string(word.begin(), word.end()));
	    word.clear();
	}
	// Find space chars
        else if (whitespace.find(c) != string::npos) {
	    string s = string(word.begin(), word.end());
	    if(s.length() > 0) {
	    	words->push_back(s);
	    	word.clear();
	    }
	}
	// Find unwanted chars
	else if (UNWANTED_CHARACTERS.find(c) != string::npos) {
	    continue;
	}
	// Normal chars
	else
	    word.push_back(c);
    }
    if (word.size() > 0) {
	words->push_back(string(word.begin(), word.end()));
    }
    return words;
  };

/**
 * Load a file and return a vector of all the words in that file
 * This function only splits the contents of the file over whitespace
 * and does not make exceptions for punctuation or unwanted characters
 * Caller must delete returned vector
 */
  vector<string> * getWords(const string & dirLocation) {
    vector<string> * words = new vector<string>();
    words->push_back(" ");
    ifstream infile;	
    string line;
    DIR* dir = opendir(dirLocation.c_str());
    struct dirent* ent;
    string fileLocation;
    while((ent = readdir(dir)) != nullptr){
	fileLocation = ent->d_name;
	if(fileLocation == "." || fileLocation == "..")
	    continue;
	infile.open(dirLocation + "/" + fileLocation);
	if (!infile)
	    exit(1);
	while(getline(infile, line)) {
	    vector<string> * wordsInLine = tokenize(line);
	    words->insert(words->end(), wordsInLine->begin(), wordsInLine->end());
	    delete wordsInLine;
	}
	infile.close();
	words->push_back(" ");
    }
    closedir(dir);
    return words;
  };

private:
  const string whitespace = " \t\r\n\v\f";
  const string PUNCTUATION = ".!,?";
  const string UNWANTED_CHARACTERS = ";:\"~()[]{}\\/^_<>*=&%@$+|`";

};

#endif //DOCUMENT_GENERATOR_HPP
