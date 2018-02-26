#include <cstdlib>

#include <iostream>
#include <fstream>
#include <chrono>

#include <AttributeComparator.h>
#include <AttributeDeconstructor.h>
#include <Graph.h>
#include <MatchingMachine.h>
#include <MaMaConstrFirst.h>
#include <Match.h>
#include <Solver.h>
#include <IsoGISolver.h>
#include <SubGISolver.h>
#include <InducedSubGISolver.h>

#include <c_textdb_driver.h>

const rilib::MATCH_TYPE matchtype = rilib::MT_INDSUB;
//const rilib::MATCH_TYPE matchtype = rilib::MT_MONO;

unsigned int read1(std::istream & in) {
  unsigned char a, b;
  a = static_cast<unsigned char>(in.get());
  b = static_cast<unsigned char>(in.get());
  return a | (b << 8);
}


void read_amalfi(std::istream & in, rilib::Graph * graph) {
  using namespace rilib;

  int i=0, j=0;

  graph->nof_nodes = read1(in);

  graph->nodes_attrs = (void**)malloc(graph->nof_nodes * sizeof(void*));
  for(i=0; i<graph->nof_nodes; ++i){
    graph->nodes_attrs[i] = NULL;
  }

  graph->out_adj_sizes = (int *)calloc(graph->nof_nodes,sizeof(int));
  graph->in_adj_sizes = (int *)calloc(graph->nof_nodes,sizeof(int));

  gr_neighs_t **ns_o = (gr_neighs_t **)malloc(graph->nof_nodes * sizeof(gr_neighs_t *));
  for(i=0; i<graph->nof_nodes; i++){
    ns_o[i] = NULL;
  }

  for(int u=0; u<graph->nof_nodes; ++u) {
    for(auto cnt=read1(in); cnt>0; --cnt) {
      auto v = read1(in);

      ++graph->out_adj_sizes[u];
      ++graph->in_adj_sizes[v];

      if(ns_o[u] == NULL){
        ns_o[u] = (gr_neighs_t*)malloc(sizeof(gr_neighs_t));
        ns_o[u]->nid = v;
        ns_o[u]->next = NULL;
      }
      else{
        gr_neighs_t* n = (gr_neighs_t*)malloc(sizeof(gr_neighs_t));
        n->nid = v;
        n->next = ns_o[u];
        ns_o[u] = n;
      }
    }
  }


  graph->out_adj_list = (int**)malloc(graph->nof_nodes * sizeof(int*));
  graph->in_adj_list = (int**)malloc(graph->nof_nodes * sizeof(int*));
  graph->out_adj_attrs = (void***)malloc(graph->nof_nodes * sizeof(void**));

  int* ink = (int*)calloc(graph->nof_nodes, sizeof(int));
  for (i=0; i<graph->nof_nodes; i++){
    graph->in_adj_list[i] = (int *)calloc(graph->in_adj_sizes[i], sizeof(int));

  }
  for (i=0; i<graph->nof_nodes; i++){
    // reading degree and successors of vertex i
    graph->out_adj_list[i] = (int*)calloc(graph->out_adj_sizes[i], sizeof(int));
    graph->out_adj_attrs[i] = (void**)malloc(graph->out_adj_sizes[i] * sizeof(void*));

    gr_neighs_t *n = ns_o[i];
    for (j=0; j<graph->out_adj_sizes[i]; j++){
      graph->out_adj_list[i][j] = n->nid;
      graph->out_adj_attrs[i][j] = NULL;

      graph->in_adj_list[n->nid][ink[n->nid]] = i;

      ink[n->nid]++;

      n = n->next;
    }
  }

  for(int i=0; i<graph->nof_nodes; i++){
    if(ns_o[i] != NULL){
      gr_neighs_t *p = NULL;
      gr_neighs_t *n = ns_o[i];
      for (j=0; j<graph->out_adj_sizes[i]; j++){
        if(p!=NULL)
          free(p);
        p = n;
        n = n->next;
      }
      if(p!=NULL)
      free(p);
    }
  }
}

int main(int argc, char *argv[]) {
  using namespace std;
  using namespace rilib;

  char * pattern_filename = argv[1];
  char * target_filename = argv[2];

  AttributeComparator* nodeComparator = new DefaultAttrComparator();
  AttributeComparator* edgeComparator = new DefaultAttrComparator();
  AttributeDeconstructor* nodeAttrDeco = new VoidAttrDeCo();
  AttributeDeconstructor* edgeAttrDeco = new VoidAttrDeCo();


  ifstream in{pattern_filename, ios::in|ios::binary};
  if(!in.is_open()) return 1;
  Graph * pattern = new Graph();
  read_amalfi(in,pattern);
  
  in.close();

  MaMaConstrFirst* mama = new MaMaConstrFirst(*pattern);
  mama->build(*pattern);

  //mama->print();

  in.open(target_filename, ios::in|ios::binary);
  if(!in.is_open()) return 1;
  Graph * target = new Graph();
  read_amalfi(in,target);
  in.close();

  MatchListener * matchListener = new EmptyMatchListener();

  chrono::time_point<chrono::steady_clock> start, end;
  chrono::milliseconds elapsed;

  start = chrono::steady_clock::now();

  long tsteps = 0, ttriedcouples = 0, tmatchedcouples = 0;
  match(
      *target,
      *pattern,
      *mama,
      *matchListener,
      matchtype,
      *nodeComparator,
      *edgeComparator,
      &tsteps,
      &ttriedcouples,
      &tmatchedcouples);

  auto count = matchListener->matchcount;

  end = chrono::steady_clock::now();
  elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);

  delete matchListener;

  cout << count << "," << elapsed.count() << endl;
}
