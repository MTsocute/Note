#include "include/algraph.h"
#include "../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX15.0.sdk/usr/include/c++/v1/tuple"

void directG3() {
    vector<std::tuple<int, int, int>>  directed_edges = {
        {0, 2, 1},
        {1, 2, 1},
        {2, 3, 1},
        {2, 4, 1},
        {4, 3, 1},
    };

    auto alg = initialize_adjacency_list_graph_directed("ABCDE", directed_edges);

    TopoSort(alg);
}

int main() {
    directG3();

    return 0;
}
