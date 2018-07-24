#ifndef READ_AMALFI_H_
#define READ_AMALFI_H_

#include <cstdint>
#include <istream>

#include <Graph.h>
#include <c_textdb_driver.h>

inline uint16_t read_word(std::istream & in) {
  uint16_t x = static_cast<unsigned char>(in.get());
  x |= static_cast<uint16_t>(in.get()) << 8;
  return x;
}

inline void read_amalfi(std::istream & in, rilib::Graph * graph) {
  using namespace rilib;

  int i=0, j=0;

  graph->nof_nodes = read_word(in);

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
    for(auto cnt=read_word(in); cnt>0; --cnt) {
      auto v = read_word(in);

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

#endif  // READ_AMALFI_H_
