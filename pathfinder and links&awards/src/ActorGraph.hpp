/*
 * ActorGraph.hpp
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <unordered_set>

using namespace std;

class Movie {
  public:
    string name;
    int year;
    int weight; 
    unordered_map<string, int> performer; // <movie name, actor name>

    // Set the performer
    Movie(string &n, string &m, int y, int w): name(m), year(y) {
        weight = 1 + ((2018 - y) * w);
	performer.insert( make_pair(n, weight) );
    };
};

struct Vertex {
    vector<pair<string, pair<Vertex*, pair<Movie*, int>>>> rel;
    set<Vertex*> worked;
    string name;
    unsigned int dist = 100000000;
    Vertex *prev = nullptr;
    bool done = false;
    int link = 0;
    string output = "";
    
    Vertex(const string &n) {
        name = n;
    }
    
    void addRel(const string &s, Vertex *v, Movie *mov) {
        rel.push_back ( make_pair(s, make_pair(v,
                        make_pair(mov, mov->weight))) );
    }

    void addWorked(Vertex *v) {
        worked.insert(v);
    }
    
    void clean() {
        dist = 100000000;
        prev = nullptr;
        done = false;
    }
};

struct Node {
    int w = 0;
    Vertex *v;
    Node(int w, Vertex *ver) {
        this->w = w;
        v = ver;
    };
};

struct cmpw {
    bool operator()(const Node a, const Node b) {
        return a.w > b.w;
    }
};

struct cmpr {
    bool operator()(const Vertex *a, const Vertex *b) {
        return a->link >= b->link;
    }
};

struct cmpn {
    bool operator()(const string &s1,const string &s2){
        if (s1 == s2)
            return s1.compare(s2)<0;
        return s1 > s2;
    }
};

class ActorGraph {
  
  public:
    // Graph that record all the actors/actress
    unordered_map<string, Vertex*> graph;

    // Record all the movie actors
    multimap<pair<string, int>, Movie*>  movies; // <Movie name, vector of actors name>

    ActorGraph(void);

    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2018 -
     * movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char *in_filename, const bool use_weighted_edges);

    bool loadFile(const char *in_filename);

    string findPath(const string *start, const string *end);

    void generateList(int k, const char *out_filename);
    
    void clear();
    
    ~ActorGraph();

};


/*
bool static sortv(const pair<int,string> &p1,const pair<int,string> &p2){
  if(p1.first==p2.first)
    return p1.second.compare(p2.second)<0;
  return p1.first>p2.first;
}*/

#endif  // ACTORGRAPH_HPP
