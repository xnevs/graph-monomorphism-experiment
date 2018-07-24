#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>

#include <AttributeComparator.h>
#include <Graph.h>
#include <MatchingMachine.h>
#include <MaMaConstrFirstDs.h>
#include <MatchListener.h>
#include <Solver.h>
#include <SubGISolver.h>
#include <InducedSubGISolver.h>
#include <Domains.h>

#include <read_ldgraphs.h>

int main(int argc, char *argv[]) {
  using namespace std;
  using namespace rilib;

  char * pattern_filename = argv[1];
  char * target_filename = argv[2];

  AttributeComparator* nodeComparator = new IntAttrComparator();
  AttributeComparator* edgeComparator = new DefaultAttrComparator();

  ifstream in{pattern_filename, ios::in|ios::binary};
  if(!in.is_open()) return 1;
  Graph * pattern = new Graph();
  read_ldgraphs(in,pattern);
  in.close();

  in.open(target_filename, ios::in|ios::binary);
  if(!in.is_open()) return 1;
  Graph * target = new Graph();
  read_ldgraphs(in,target);
  in.close();

  MatchListener * matchListener = new EmptyMatchListener();

  sbitset *domains = new sbitset[pattern->nof_nodes];

  chrono::time_point<chrono::steady_clock> start, end;
  chrono::milliseconds elapsed;

  start = chrono::steady_clock::now();

  bool domok = init_domains(*target, *pattern, *nodeComparator, *edgeComparator, domains, false);

  if(domok) {
    int *domains_size = new int[pattern->nof_nodes];
    int dsize;
    for(int ii=0; ii<pattern->nof_nodes; ii++){
      dsize = 0;
      for(sbitset::iterator IT = domains[ii].first_ones(); IT!=domains[ii].end(); IT.next_ones()){
        dsize++;
      }
      domains_size[ii] = dsize;
    }

    MatchingMachine* mama = new MaMaConstrFirstDs(*pattern, domains, domains_size);
    mama->build(*pattern);

    Solver * solver = new InducedSubGISolver(*mama, *target, *pattern, *nodeComparator, *edgeComparator, *matchListener, domains, domains_size);

    solver->solve();
  }

  auto count = matchListener->matchcount;

  end = chrono::steady_clock::now();
  elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);

  delete matchListener;

  cout << count << "," << elapsed.count() << endl;
}
