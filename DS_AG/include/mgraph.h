#ifndef GRAPH_H
#define GRAPH_H
#define INFIN INT_MAX

#include "../include/stdc++.h"

using namespace std;

const int MAX_SIZE = 100;  // 假设图的最大节点数量

// 邻接矩阵图
struct MGraph {
    char vertices[MAX_SIZE];  // 存储顶点
    int edges[MAX_SIZE][MAX_SIZE];  // 邻接矩阵，存储边的权重
    int vertex_count;  // 顶点数量
};

// 邻接表中的边节点
struct ArcNode {
    int weight;     // 权重
    int adjacent_vertex_index;  // 邻接顶点的索引
    ArcNode *next;  // 指向下一个邻接点
};

// 邻接表图中的顶点
struct VertexNode {
    char data;  // 顶点信息
    ArcNode *first_edge;  // 邻接表中的第一个边节点
};

// 邻接表图
struct ALGraph {
    VertexNode adj_list[MAX_SIZE];  // 邻接表
    int vertex_count;  // 顶点数量
    int edge_count;  // 边的数量
};

// 函数声明
MGraph initialize_adjacency_matrix_graph(const string &vertex_list, const vector<tuple<int, int, int>> &edges);
MGraph initialize_adjacency_matrix_graph_directed(const string &vertex_list, const vector<tuple<int, int, int>> &edges);
void DFS_MGraph(MGraph *mg, int start_vertex, vector<bool> &visited);
void DFS(MGraph &mg, int start_vertex);
void display_MGraph(MGraph *mg);
void BFS(MGraph &mg, int start_vertex);
void BFS(MGraph &mg, const int &start_vertex, vector<bool> &visited, queue<int> &q);
void BFS_Min_Distance(MGraph &mg, const int start_vertex);
void Dijistra_Min_Distance(MGraph & mg, const int start);
void Floyd_Min_Distance(MGraph & mg);
#endif  // GRAPH_H