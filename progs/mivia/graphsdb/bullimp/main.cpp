#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>

#include <boost/graph/adjacency_list.hpp> 

#include "ullimp/graph/ullimp_sub_graph_iso.hpp"

unsigned int read1(std::istream & in) {
    unsigned char a, b;
    a = static_cast<unsigned char>(in.get());
    b = static_cast<unsigned char>(in.get());
    return a | (b << 8);
}

template<typename R>
R read_amalfi(std::istream & in) {
    using namespace boost;

    auto n = read1(in);
    R g{n};
    for(int i=0; i<n; i++) {
        auto cnt = read1(in);
        for(int j=0; j<cnt; j++) {
            auto k = read1(in);
            add_edge(i,k,g);
        }
    }
    return g;
}

using graph_type =  boost::adjacency_list<boost::setS, boost::vecS, boost::bidirectionalS>; 

int main(int argc, char *argv[]) { 
    using namespace std;

    char * pattern_filename = argv[1];
    char * target_filename = argv[2];

    ifstream in{pattern_filename, ios::in|ios::binary};
    if(!in.is_open()) return 1;

    auto pattern = read_amalfi<graph_type>(in);
    in.close();

    in.open(target_filename, ios::in|ios::binary);
    if(!in.is_open()) return 1;

    auto graph = read_amalfi<graph_type>(in);
    in.close();

    chrono::time_point<chrono::steady_clock> start, end;
    chrono::milliseconds elapsed;

    start = chrono::steady_clock::now();

    int count = 0;
    ullimp::ullimp_subgraph_iso(
            pattern,
            graph,
            [&count](auto const &a, auto const &b){
                ++count;
                return true;
            });

    end = chrono::steady_clock::now();
    elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << count << "," << elapsed.count() << endl;
}
