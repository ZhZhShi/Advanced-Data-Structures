#include "ActorGraph.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  
  if (argc != 4) {
    cerr<<argv[0]<<" called with incorrect arguments.\n"
        <<"Usage: "<<argv[0]<<" movie_cast_tsv_file k pairs_tsv_file output_paths_tsv_file\n" ;
    exit(1);
  }

  ActorGraph *graph = new ActorGraph();
  graph->loadFile(argv[1]);

  char *eptr;
  int k = strtol(argv[2], &eptr, 10);
  cout<<k<<'\n';
  
  graph->generateList(k, argv[3]);

  delete graph;

  return 0;
}
