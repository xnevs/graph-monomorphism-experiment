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

#include <read_amalfi.h>

const rilib::MATCH_TYPE matchtype = rilib::MT_INDSUB;
//const rilib::MATCH_TYPE matchtype = rilib::MT_MONO;

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
