#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {

  if(argc!=5){
    cerr<<argv[0]<<" called with incorrect arguments.\n"
        <<"Usage: "<<argv[0]<<" movie_cast_tsv_file u/w pairs_tsv_file output_paths_tsv_file\n" ;
    exit(1);
  }
 
  ActorGraph *graph = new ActorGraph();
  if(strcmp("u",argv[2])!=0&&strcmp(argv[2],"w")!=0){
    cerr<<"wrong parameter '" << argv[2] <<"', must be u or w\n";
    exit(1);
  }
  graph->loadFromFile(argv[1],(strcmp(argv[2],"u")==0)?0:1);

  ifstream infile(argv[3]);
  ofstream outfile(argv[4]);
  bool head=false;

  if(!infile.is_open()){
    cerr<<"Failed to read "<<argv[3]<<"!\n";
    exit(1);
  }

  while(infile){
    string s;

    if(!getline(infile,s))
      break;

    if(!head){
      head=true;
      outfile<<"(actor)--[movie#@year]-->(actor)--...\n";
      outfile.flush();
      continue;
    }
    
    istringstream ss(s);
    vector<string> record;    

    while(ss){
      string next;
      if(!getline(ss,next,'\t'))
        break;
      record.push_back(next);
    }
   
    cerr<<"Computing path for ("<<record[0]<<") -> ("<<record[1]<<")\n";
    string ret = graph->findPath(&record[0],&record[1]);
    outfile<<ret;
    outfile.flush();

  }
  infile.close();
  outfile.close();
  delete graph;
  return 0;
}


