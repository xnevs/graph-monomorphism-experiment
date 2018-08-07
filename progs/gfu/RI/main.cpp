#include <cstdlib>

#include <iostream>
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

int main(int argc, char *argv[]) {
  using namespace std;
  using namespace rilib;

  char * pattern_filename = argv[1];
  char * target_filename = argv[2];

  AttributeComparator* nodeComparator = new StringAttrComparator();
  AttributeComparator* edgeComparator = new DefaultAttrComparator();
  AttributeDeconstructor* nodeAttrDeco = new StringAttrDeCo();
  AttributeDeconstructor* edgeAttrDeco = new VoidAttrDeCo();

  Graph * pattern = new Graph();
  read_graph(pattern_filename, pattern, GFT_GFU);

  MaMaConstrFirst* mama = new MaMaConstrFirst(*pattern);
  mama->build(*pattern);

  //mama->print();

  Graph * target = new Graph();
  read_graph(target_filename, target, GFT_GFU);

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
