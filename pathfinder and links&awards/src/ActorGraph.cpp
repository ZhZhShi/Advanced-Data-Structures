/*
 * ActorGraph.cpp
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format defined in movie_casts.tsv.
 *
 * Feel free to modify any/all aspects as you wish.
 */
#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <string.h>

using namespace std;

ActorGraph::ActorGraph(void) {}

/**
 * Build performers graph with each performer as a node and movie as edge.
 */
bool ActorGraph::loadFromFile(const char *in_filename,
                              const bool use_weighted_edges) {
    
    // int edge = 0;

    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
    
    cout << "Reading " << in_filename << " ...\n";

    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
        
        // get the next line
        if (!getline(infile, s)) break;
        
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
        
        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string next;

            // get the next string before hitting a tab character and put it in 'next'
            if (!getline(ss, next, '\t')) break;

            record.push_back(next);
        }

    
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);

        // Set weight
        int weight = 0;
        if (use_weighted_edges)
            weight = 1;

        // Store the movie
        auto it = movies.find(make_pair(movie_title, movie_year));
        // New movie
        if (it == movies.end()) {
            Movie *movie = new Movie(actor_name, movie_title, movie_year, weight);
            movies.insert( make_pair(make_pair(movie_title, movie_year), movie) );
            // Actor not found
            if (graph.find(actor_name) == graph.end())
                graph.insert( make_pair(actor_name, new Vertex(actor_name)) );
        }
        // Movie existed
        else {
            Movie *movie = it->second;
            // Actor not found
            if (graph.find(actor_name) == graph.end())
                graph.insert( make_pair(actor_name, new Vertex(actor_name)) );
            Vertex *actor = graph[actor_name];
            for (auto pit = movie->performer.begin();
                 pit != movie->performer.end(); ++pit) {
                actor->addRel(pit->first, graph[pit->first], movie);
                actor->link++;
                Vertex *act = graph[pit->first];
                act->addRel(actor_name, actor, movie);
                act->link++;
                // edge += 2;
            }
            movie->performer.insert( make_pair(actor_name, movie->weight) );
        }
    }
/*
    cout << "#node: " << graph.size() << "\n";
    cout << "#movies: " << movies.size() << "\n";
    cout << "#edges: " << edge << "\n";
    cout << "done\n";
*/
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}



/**
 * Load file by using Vertex->worked
 */
bool ActorGraph::loadFile(const char *in_filename) {

    // int edge = 0;

    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
    
    cout << "Reading " << in_filename << " ...\n";

    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
        
        // get the next line
        if (!getline(infile, s)) break;
        
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
        
        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string next;

            // get the next string before hitting a tab character and put it in 'next'
            if (!getline(ss, next, '\t')) break;

            record.push_back(next);
        }

    
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);

        // Set weight
        int weight = 0;

        // Store the movie
        auto it = movies.find(make_pair(movie_title, movie_year));
        // New movie
        if (it == movies.end()) {
            Movie *movie = new Movie(actor_name, movie_title, movie_year, weight);
            movies.insert( make_pair(make_pair(movie_title, movie_year), movie) );
            // Actor not found
            if (graph.find(actor_name) == graph.end())
                graph.insert( make_pair(actor_name, new Vertex(actor_name)) );
        }
        // Movie existed
        else {
            Movie *movie = it->second;
            // Actor not found
            if (graph.find(actor_name) == graph.end())
                graph.insert( make_pair(actor_name, new Vertex(actor_name)) );
            Vertex *actor = graph[actor_name];
            for (auto pit = movie->performer.begin();
                 pit != movie->performer.end(); ++pit) {
                actor->addWorked(graph[pit->first]);
                Vertex *act = graph[pit->first];
                act->addWorked(actor);
                // edge += 2;
            }
            movie->performer.insert( make_pair(actor_name, movie->weight) );
        }
    }
/*
    cout << "#node: " << graph.size() << "\n";
    cout << "#movies: " << movies.size() << "\n";
    cout << "#edges: " << edge << "\n";
    cout << "done\n";
*/
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }

    infile.close();

    return true;

}



/**
 * Find the shortest path between two performers.
 */
string ActorGraph::findPath(const string *start, const string *end) {
    
    if (graph.find(*start) == graph.end()) {
        // cerr << "Cannot find " + *start + '\n';
        exit(1);
    }
    if (graph.find(*end) == graph.end()) {
        // cerr << "Cannot find " + *end + '\n';
        exit(1);
    }
    
    priority_queue<Node, vector<Node>, cmpw> pq;
    
    for (auto git = graph.begin(); git != graph.end(); ++git)
        git->second->clean();
    
    Vertex *root = graph[*start];
    root->dist = 0;
    pq.push(Node(0, root));
    
    while (!pq.empty()) {
        Node n = pq.top();
        pq.pop();
        if (n.v->done == false) {
            n.v->done = true;
            for ( auto rit = n.v->rel.begin(); rit != n.v->rel.end(); ++rit ) {
                unsigned int dist = n.v->dist + rit->second.second.second;
                Vertex *w = graph[rit->first];
                if (dist < w->dist) {
                    w->prev = n.v;
                    w->dist = dist;
                    w->output = "--[" + rit->second.second.first->name + "#@" +
                                to_string(rit->second.second.first->year) +
                                "]-->(" + w->name + ")";
                    pq.push(Node(w->dist, w));
                }
            }
        }
    }
           
    Vertex *curr = graph[*end];
    string ret = "";
    while ( curr->prev != nullptr ) {
        ret = curr->output + ret;
        curr = curr->prev;
    }
    ret = "(" + curr->name + ")" + ret;
    ret = ret+"\n";
    return ret;

}



/**
 * Generate a list of guests how know at least k performers
 */
void ActorGraph::generateList(int k, const char *out_filename) {
    
    cout << "Preparing Invites List\n";

    priority_queue<Vertex*, vector<Vertex*>, cmpr> q;
    priority_queue<string, vector<string>, cmpn> g;
    
    for (auto git = graph.begin(); git != graph.end(); ++git) {
        git->second->link = git->second->worked.size();
        q.push(git->second);
    }
    
    // Delete all un-relative actors
    while (!q.empty()) {
        Vertex *curr = q.top();
        q.pop();
        if (curr->done) {
            continue;
        }
        curr->done = true;
        if (curr->link < k) {
            for (auto wit = curr->worked.begin(); wit != curr->worked.end(); ++wit) {
                (*wit)->link -= 1;
                q.push(*wit);
            }
        } else {
            g.push(curr->name);
        }
    }
    
    // Output all actors
    ofstream outfile(out_filename);
    outfile << "Actor\n";
    while (!g.empty()) {
	outfile << g.top() << '\n';
	g.pop();
    }
    outfile.close();

}



/**
 * Destructor
 */
ActorGraph::~ActorGraph() {
    clear();
}

void ActorGraph::clear() {
    for (auto git = graph.begin(); git != graph.end(); ++git)
        delete git->second;
    for (auto mit = movies.begin(); mit != movies.end(); mit++ )
        delete mit->second;
}
