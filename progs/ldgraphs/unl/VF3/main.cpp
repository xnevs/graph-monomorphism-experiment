#include <iostream>
#include <fstream>
#include <chrono>

#include <match.hpp>
#include <argloader.hpp>
#include <argraph.hpp>
#include <argedit.hpp>
#include <nodesorter.hpp>
#include <probability_strategy.hpp>
#include <vf3_sub_state.hpp>
#include <nodesorter.hpp>
#include <nodeclassifier.hpp>


template<> long long VF3SubState<int,int,Empty,Empty>::instance_count = 0;

bool count_visitor(int n, node_id ni1[], node_id ni2[], void * state, void * count) {
  (*static_cast<int *>(count))++;
  return false;
}

int main(int argc, char *argv[]) {
  using namespace std;

  char * pattern_filename = argv[1];
  char * target_filename = argv[2];

  ifstream in;

  in.open(pattern_filename,ios::in|ios::binary);
  StreamARGLoader<int, Empty> pattern_loader(in);
  ARGraph<int, Empty> pattern(&pattern_loader);
  in.close();

  in.open(target_filename,ios::in|ios::binary);
  StreamARGLoader<int, Empty> target_loader(in);
  ARGraph<int, Empty> target(&target_loader);
  in.close();

  chrono::time_point<chrono::steady_clock> start, end;
  chrono::milliseconds elapsed;

  start = chrono::steady_clock::now();

  int n;
  int nodes1, nodes2;
  nodes1 = pattern.NodeCount();
  nodes2 = target.NodeCount();
  node_id *n1, *n2;
  n1 = new node_id[nodes1];
  n2 = new node_id[nodes2];

  NodeClassifier<int,Empty> classifier(&target);
  NodeClassifier<int,Empty> classifier2(&pattern, classifier);
  std::vector<int> class_patt = classifier2.GetClasses();
  std::vector<int> class_targ = classifier.GetClasses();

  VF3NodeSorter<int, Empty, SubIsoNodeProbability<int, Empty> > sorter(&target);
  std::vector<node_id> sorted = sorter.SortNodes(&pattern);

  VF3SubState<int, int, Empty, Empty> s0(
      &pattern, &target, class_patt.data(), class_targ.data(),
      classifier.CountClasses(), sorted.data());

  int count = 0;
  match<VF3SubState<int, int, Empty, Empty> >(s0, &n, n1, n2, count_visitor, &count);

  end = chrono::steady_clock::now();
  elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);

  cout << count << "," << elapsed.count() << endl;

  return 0;
}
