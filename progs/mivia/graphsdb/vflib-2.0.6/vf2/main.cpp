#include <fstream>
#include <chrono>

#include <argraph.h>
#include <argloader.h>
#include <vf2_sub_state.h>
#include <match.h>

bool count_visitor(int n, node_id ni1[], node_id ni2[], void * count) {
    (*static_cast<int *>(count))++;
    return false;
}

int main(int argc, char *argv[]) {
    using namespace std;

    char * pattern_filename = argv[1];
    char * target_filename = argv[2];

    ifstream in;

    in.open(pattern_filename,ios::in|ios::binary);
    BinaryGraphLoader pattern_loader{in};
    Graph pattern{&pattern_loader};
    in.close();

    in.open(target_filename,ios::in|ios::binary);
    BinaryGraphLoader target_loader{in};
    Graph target{&target_loader};
    in.close();

    chrono::time_point<chrono::steady_clock> start, end;
    chrono::milliseconds elapsed;

    start = chrono::steady_clock::now();

    VF2SubState s0{&pattern,&target};
    int count = 0;
    match(&s0,count_visitor,&count);

    end = chrono::steady_clock::now();
    elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << count << "," << elapsed.count() << endl;

    return 0;
}
