#include "ActorGraph.hpp"
#include "MatrixMultiply.hpp"
#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>


using namespace std;

bool static sortv(const pair<int,string> &p1,const pair<int,string> &p2){
  if(p1.first==p2.first)
    return p1.second.compare(p2.second)<0;
  return p1.first>p2.first;
}
int main(int argc, char *argv[]) {
  
  if(argc!=5){
   // cerr<<argv[0]<<" called with incorrect arguments.\n"<<"Usage: "<<argv[0]<<" movie_cast_tsv_file actor_list actor_existing_predictions actor_new_predictions" ;
    exit(1);
  }

  ActorGraph *graph = new ActorGraph();
  graph->loadFile(argv[1]);
  
  ifstream infile(argv[2]);

  if(!infile.is_open()){
    //cerr<<"Failed to read "<<argv[2]<<"!\n";
    exit(1);
  }

  ofstream future(argv[3]);
  ofstream newcol(argv[4]);
  
  vector<vector<int>> relation;
  unordered_map<string,int> index;
  unordered_map<int,string> rindex;
 
  unordered_map<string,Vertex*>::iterator it=graph->graph.begin();
  for(unsigned int i=0; i<graph->graph.size();i++){
    index.insert(make_pair(it->first,i));
    rindex.insert(make_pair(i,it->first)); 
    it++;
  }/*
  auto itt = index.begin();
  for(;itt!=index.end();itt++)
    cerr<<itt->first<<itt->second<<"\n";
  */
  it = graph->graph.begin();
  for(; it!=graph->graph.end();it++){
    vector<int> inner(graph->graph.size());
    auto itr = it->second->worked.begin();
    for(; itr!=it->second->worked.end();itr++){
      inner[index[(*itr)->name]]=1;
    }
    relation.push_back(inner);
  }
  
  vector<vector<int>> inputm;
  unordered_map<string,int> iindex;
  int count=0;
  int head=1;
  while (infile){
    string line;
    if(!getline (infile,line))
      break;
    if (head){
      head=0;
      continue;
    } 
    if(index.find(line)==index.end()){
      exit(1);
    }
    inputm.push_back(relation[index[line]]);
    iindex.insert(make_pair(line,count));
    count++;
  }
  MatrixOperations<int> *matrix = new MatrixOperations<int>(inputm,relation);
  auto cone = matrix->matrixMultiply(); 
   
  head=1;

  infile.close();
  ifstream inf(argv[2]);

  while (inf){
    string line;
    if (!getline (inf ,line))
      break;
 
    if (head ){ 
      head=0;
      future<<"Actor1,Actor2,Actor3,Actor4\n";
      newcol<<"Actor1,Actor2,Actor3,Actor4\n";
      future.flush();
      newcol.flush();
      continue;
      }

   // cerr<<"Computing predictions for ("<<line<<")\n";
    
    if(index.find(line)==index.end()){
     // cerr<<"Failed to locate node '"<<line<<"'\n"<<argv[0]<<" called with incorrect arguments.\n";
      //cerr<<"Usage: "<<argv[0]<<" movie_cast_tsv_file actor_list actor_existing_predictions actor_new_predictions\n";
      exit(1);
    }

    auto  &curr=inputm[iindex[line]];
    vector<pair<int,string>> oldresult;
    vector<pair<int,string>> newresult;
    for(int i=0;(unsigned int)i<curr.size();i++){
      if(line==rindex[i])
        continue;
      if(curr[i]==1)
        oldresult.push_back(make_pair(cone[iindex[line]][i],rindex[i] ));
      else if(cone[iindex[line]][i]!=0)
        newresult.push_back(make_pair(cone[iindex[line]][i],rindex[i] ));
    }
                 
    vector<pair<int,string>> oldret;
    vector<pair<int,string>> newret;
    int thresh=0;
    auto itrr=oldresult.begin();
    for(;itrr!=oldresult.end();itrr++){
      if(oldret.size()<4||itrr->first>=thresh)
        oldret.push_back(*itrr);
      if(oldret.size()>4){
        sort(oldret.begin(),oldret.end(),sortv);
        oldret.pop_back();
        thresh=oldret[3].first;
      }
    }
    thresh=0;
    itrr=newresult.begin();
    for(;itrr!=newresult.end();itrr++){
      if(newret.size()<4||itrr->first>=thresh)
        newret.push_back(*itrr);
      if(newret.size()>4){
        sort(newret.begin(),newret.end(),sortv);
        newret.pop_back();
        thresh=newret[3].first;
      }
    }
      
    int limit = (oldret.size()<4)?oldret.size():4;
    string ret="";
    for(int i=0; i<limit; i++){
      ret = ret + oldret[i].second;
      ret = ret + "\t";
    }
    ret = ret + "\n";
    future<<ret;
    future.flush();
   
    limit = (newret.size()<4)?newret.size():4;
    ret = "";
    for(int i=0; i<limit;i++){
      ret = ret + newret[i].second;
      ret = ret + "\t";
    }
    ret = ret + "\n";
    newcol<<ret;
    newcol.flush();     
  } 
    

/*  int head=1;

  while(infile){  
    string line;
    if(!getline(infile,line))
      break;

    if(head){
      head=0; 
      future<<"Actor1,Actor2,Actor3,Actor4\n";
      newcol<<"Actor1,Actor2,Actor3,Actor4\n";
      future.flush();
      newcol.flush();
      continue;
    } 

    cerr<<"Computing predictions for ("<<line<<")\n";
    
    if(graph->graph.find(line)==graph->graph.end()){
      cerr<<"Failed to locate node '"<<line<<"'\n"<<argv[0]<<" called with incorrect arguments.\n";
      cerr<<"Usage: "<<argv[0]<<" movie_cast_tsv_file actor_list actor_existing_predictions actor_new_predictions\n";
      exit(1);
    }

    
    //vector<int> actor=relation[index[line]];
    vector<pair<int,string>> oldresult;
    vector<pair<int,string>> newresult;
    vector<string> inserted;*/
    /*for(int i=0; (unsigned int)i<actor.size(); i++){
      vector<vector<int>> column;
      for(int j=0; (unsigned int)j<relation.size();j++){
        vector<int> col={relation[j][i]};
        column.push_back(col);
      }
      vector<vector<int>> row;
      row.push_back(actor);
      MatrixOperations<int> *matrix = new MatrixOperations<int>(row,column);
      vector<vector<int>> common = matrix->matrixMultiply();
      if(i==index[line])
        continue;
      if(actor[i]==0&&common[0][0]!=0)
        newresult.push_back(make_pair(common[0][0],rindex[i]));
      if(actor[i]==1)
        oldresult.push_back(make_pair(common[0][0],rindex[i]));
      delete matrix;
    }*/

  /*  set<Vertex*> &curr = graph->graph[line]->worked;
    auto itr = curr.begin();
    for(;itr!=curr.end();itr++){*/
      /*int common=0;
      auto setit = (*itr)->worked.begin();
      //cerr<<"oldcompare start"<<"\n";
      for(;setit!=(*itr)->worked.end();setit++){
        if(curr.find(*setit)!=curr.end())
          common++;
      }*/
      //cerr<<"oldcompare end"<<"\n";
      /*set<Vertex*> inter;
      set_intersection(curr.begin(),curr.end(),(*itr)->worked.begin(),(*itr)->worked.end(),inserter(inter,inter.begin()));
      
      oldresult.push_back(make_pair(inter.size(),(*itr)->name));
      
      auto itt = (*itr)->worked.begin();
      for(;itt!=(*itr)->worked.end();itt++){
        if(curr.find(*itt)!=curr.end()||(*itt)->name==line||find(inserted.begin(),inserted.end(),(*itt)->name)!=inserted.end())
          continue;
        set<Vertex*> intersec;*/
        //int commoon=0;
        //cerr<<"newcompare start"<<"\n";
        /*auto setitr = (*itt)->worked.begin();
        for(;setitr!=(*itt)->worked.end();setitr++){
          if(curr.find(*setitr)!=curr.end())
            commoon++;
        }*/
        //cerr<<"newcompare end"<<"\n";
         /*set_intersection(curr.begin(),curr.end(),(*itt)->worked.begin(),(*itt)->worked.end(),inserter(intersec,intersec.begin()));
        if(intersec.size()!=0){
          newresult.push_back(make_pair(intersec.size(),(*itt)->name));
          inserted.push_back((*itt)->name);
        }
      }
    }

    vector<pair<int,string>> oldret;
    vector<pair<int,string>> newret;
    int thresh=0;
    auto itrr=oldresult.begin();
    for(;itrr!=oldresult.end();itrr++){
      if(oldret.size()<4||itrr->first>=thresh)
        oldret.push_back(*itrr);
      if(oldret.size()>4){
        sort(oldret.begin(),oldret.end(),sortv);
        oldret.pop_back();
        thresh=oldret[3].first;
      }
    }
    thresh=0;
    itrr=newresult.begin();
    for(;itrr!=newresult.end();itrr++){
      if(newret.size()<4||itrr->first>=thresh)
        newret.push_back(*itrr);
      if(newret.size()>4){
        sort(newret.begin(),newret.end(),sortv);
        newret.pop_back();
        thresh=newret[3].first;
      }
    }
      
    

//    sort(oldresult.begin(),oldresult.end(),sortv);
//    sort(newresult.begin(),newresult.end(),sortv);

    //int limit = (oldresult.size()<4)?oldresult.size():4;
    string ret="";
    for(int i=0; i<4; i++){
      ret = ret + oldret[i].second;
      ret = ret + "\t";
    }
    ret = ret + "\n";
    future<<ret;
    future.flush();
   
    //limit = (newresult.size()<4)?newresult.size():4;
    ret = "";
    for(int i=0; i<4;i++){
      ret = ret + newret[i].second;
      ret = ret + "\t";
    }
    ret = ret + "\n";
    newcol<<ret;
    newcol.flush();     

  }  */ 
  
  delete graph;
  delete matrix;
  inf.close();
  future.close();
  newcol.close();
  

  return 0;
}

