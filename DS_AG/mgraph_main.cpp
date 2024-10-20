#include "include/mgraph.h"

void undirected_G() {
    string vertex_list = "ABCDEFGH"; // 顶点列表
    // 边的列表
    vector<tuple<int, int, int>> edges =
        {
            {1, 2, 10},
            {2, 3, 30},
            {0, 3, 5},
            {0, 2, 10}
        };
    
     // 初始化邻接表图
    // 初始化邻接矩阵图
    MGraph adjacency_matrix_graph = initialize_adjacency_matrix_graph(vertex_list, edges);

    // 显示图
    display_MGraph(&adjacency_matrix_graph);

    // BFS 输出所有节点
    // BFS(adjacency_matrix_graph, 0);

    // 最短路径
    // BFS_Min_Distance(adjacency_matrix_graph, 0);
}

void directed_G() {
    // 有向图
    vector<tuple<int, int, int>> directed_edges = {
        {0, 1, 10},
        {0, 4, 5},
        {1, 2, 1},
        {1, 4, 2},
        {2, 3, 4},
        {3, 0, 7},
        {4, 1, 3},
        {4, 2, 9},
        {4, 3, 2},
    };

    auto adj_matrix_G = initialize_adjacency_matrix_graph_directed("ABCDE", directed_edges);

    // display_ALGraph(&adj_list_G);
    display_MGraph(&adj_matrix_G);

    Dijistra_Min_Distance(adj_matrix_G, 0);
}

void directed_G1() {
    vector<tuple<int, int, int>> directed_edges = {
        {0,1,6},
        {1,0,10},
        {0,2,13},
        {2,0,5},
        {1,2,4}
    };

    auto adj_MG = initialize_adjacency_matrix_graph_directed("123", directed_edges);

    display_MGraph(&adj_MG);

    Floyd_Min_Distance(adj_MG);

}


int main() {
    // undirected_G();
    // directed_G();

    directed_G1();

    return 0;
}