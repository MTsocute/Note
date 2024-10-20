#include "../include/mgraph.h"

// 辅助性函数声明
void init_MGraph(MGraph *mg);
void set_MGraph_vertices(MGraph *mg, const string &vertex_list);
static ArcNode *create_arc_node(int destination, int weight);
void display_MGraph(MGraph *mg);
static void min_distance_display(MGraph &mg, const int &start_vertex, const vector<vector<int>> &distances);

// 初始化邻接矩阵图
void init_MGraph(MGraph *mg) {
    for (int i = 0; i < MAX_SIZE; i++)
    {
        for (int j = 0; j < MAX_SIZE; j++)
        {   if (i == j) mg->edges[i][j] = 0;
            else mg->edges[i][j] = INFIN; // 初始化为无边
        }
    }
    mg->vertex_count = 0;
}

// 设置邻接矩阵图的顶点
void set_MGraph_vertices(MGraph *mg, const string &vertex_list)
{
    for (int i = 0; i < vertex_list.size(); i++)
    {
        mg->vertices[i] = vertex_list[i];
    }
    mg->vertex_count = vertex_list.size();
}


// 为邻接矩阵图添加边(无向)，考虑权重
void add_MGraph_edge(MGraph *mg, int src, int dest, int weight)
{
    mg->edges[src][dest] = weight; // 添加权重
    mg->edges[dest][src] = weight; // 无向图，双向边
}

// 创建邻接表中的边节点
static ArcNode *create_arc_node(int destination, int weight)
{
    ArcNode *new_node = new ArcNode;
    new_node->adjacent_vertex_index = destination;
    new_node->weight = weight; // 添加权重
    new_node->next = nullptr;
    return new_node;
}


// 为邻接矩阵图添加边（有向）
void add_MGraph_edge_directed(MGraph *mg, int src, int dest, int weight)
{
    mg->edges[src][dest] = weight; // 添加权重
}

// 邻接矩阵的初始化（有向图），考虑权重
MGraph initialize_adjacency_matrix_graph_directed(const string &vertex_list, const vector<tuple<int, int, int>> &edges)
{
    MGraph graph;
    init_MGraph(&graph);                      // 初始化邻接矩阵结构
    set_MGraph_vertices(&graph, vertex_list); // 设置顶点列表

    // 添加边
    for (const auto &edge : edges)
    {
        int src, dest, weight;
        tie(src, dest, weight) = edge;
        add_MGraph_edge_directed(&graph, src, dest, weight);
    }

    return graph;
}

// 邻接矩阵的初始化（无向图），考虑权重
MGraph initialize_adjacency_matrix_graph(const string &vertex_list, const vector<tuple<int, int, int>> &edges)
{
    MGraph graph;
    init_MGraph(&graph);                      // 初始化邻接矩阵结构
    set_MGraph_vertices(&graph, vertex_list); // 设置顶点列表

    // 添加边
    for (const auto &edge : edges)
    {
        int src, dest, weight;
        tie(src, dest, weight) = edge;
        add_MGraph_edge(&graph, src, dest, weight); // 使用无向图的添加函数
    }

    return graph;
}


// 邻接矩阵的深度优先搜索
void DFS_MGraph(MGraph *mg, int start_vertex, vector<bool> &visited)
{
    visited[start_vertex] = true;
    cout << mg->vertices[start_vertex] << " ";

    for (int i = 0; i < mg->vertex_count; i++)
    {
        if (!visited[i] && mg->edges[start_vertex][i] != 0)
        { // 检查有无边（非0权重）
            DFS_MGraph(mg, i, visited);
        }
    }
}

// BFS 的统一接口（邻接矩阵）
void BFS(MGraph &mg, int start_vertex)
{
    vector<bool> visited(mg.vertex_count, false);
    queue<int> q;

    BFS(mg, start_vertex, visited, q);
    cout << endl;
}


// 邻接矩阵的广度优先搜索
void BFS(MGraph &mg, const int &start_vertex, vector<bool> &visited, queue<int> &q)
{
    visited[start_vertex] = true;
    q.emplace(start_vertex);

    while (!q.empty())
    {
        int cur = q.front();
        q.pop();
        cout << mg.vertices[cur] << " ";

        for (int i = 0; i < mg.vertex_count; i++)
        {
            if (mg.edges[cur][i] != 0 && visited[i] == false)
            { // 检查权重是否非0
                q.push(i);
                visited[i] = true;
            }
        }
    }
}

// 显示邻接矩阵图
void display_MGraph(MGraph *mg)
{
    cout << "The adjacency matrix is: " << endl;
    cout << setw(7) << " ";
    for (int j = 0; j < mg->vertex_count; j++)
    {
        cout << setw(8) << mg->vertices[j];
    }
    cout << endl;

    for (int i = 0; i < mg->vertex_count; i++)
    {
        cout << setw(6) << mg->vertices[i] << ":";
        for (int j = 0; j < mg->vertex_count; j++) {
            if (mg->edges[i][j] == INFIN)
                cout << setw(8) << "inf"; 
            else cout << setw(8) << mg->edges[i][j];
        }
        cout << endl;
    }
}

// 深度优先搜索接口（邻接矩阵）
void DFS(MGraph &mg, int start_vertex)
{
    vector<bool> visited(mg.vertex_count, false);
    DFS_MGraph(&mg, start_vertex, visited);
    cout << endl;
}

// 辅助输出最短路径内容
static void min_distance_display(MGraph &mg, const int &start_vertex, const vector<vector<int>> &distances) {
    cout << "Begin With Index: " << start_vertex << endl;
    cout << "TO : ";
    for (int i = 0; i < mg.vertex_count; i++)
        cout << setw(5) << i << " ";
    cout << endl;

    // 输出 distance 里面的数据
    cout << "Dis: ";
    for (int i = 0; i < mg.vertex_count; i++)
    {
        if (distances[0][i] == INFIN)
            cout << setw(5) << "inf" << " "; // 用一个空格调整宽度
        else
            cout << setw(5) << distances[0][i] << " ";
    }
    cout << endl;

    cout << "Pre: ";
    for (int i = 0; i < mg.vertex_count; i++)
        cout << setw(5) << distances[1][i] << " ";
    cout << endl;
}

// BFS 实现最短路径
// 从指定节点开始到其他的节点的最短路径，默认 weight 是 1
void BFS_Min_Distance(MGraph &mg, const int start_vertex)
{
    vector<bool> visited(mg.vertex_count, false);
    queue<int> q;

    // 初始化 distance
    // d[0] 代表路径之间的 weight
    // d[1] 通往顶点的 index
    vector<vector<int>> distances(2, vector<int>(mg.vertex_count, INFIN));
    for (int i = 0; i < mg.vertex_count; i++)
        distances[1][i] = -1;

    distances[0][start_vertex] = 0; // 自己访问自己，之间的距离自然是 0

    visited[start_vertex] = true;
    q.push(start_vertex);

    while (!q.empty())
    {
        int cur = q.front();
        q.pop();

        for (int i = 0; i < mg.vertex_count; i++)
        {
            if (mg.edges[cur][i] != 0 && !visited[i])
            {                                            // 假设权重为正数
                distances[0][i] = distances[0][cur] + 1; // 更新距离
                distances[1][i] = cur;                   // 更新前驱
                q.push(i);
                visited[i] = true;
            }
        }
    }

    // 输出内容
    min_distance_display(mg, start_vertex, distances);
}

// MGraph 的 Dijistra
void Dijistra_Min_Distance(MGraph & mg, const int start) {
    // 三个表初始化
    vector<bool> visited(mg.vertex_count, false);
    vector<vector<int>> distances(2, vector<int>(mg.vertex_count, INFIN));
    distances[1] = vector<int>(mg.vertex_count, -1); // 前驱初始化为 -1

    distances[0][start] = 0; // 起点到自己的距离为 0
    // 最小堆，第一个存储 distance，第二个存储 index
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    // 塞入第一个节点
    pq.push({0, start});

    while (!pq.empty()) {
        auto [min_distance, min_index] = pq.top();
        pq.pop();

        if (visited[min_index]) continue; // 一开始就找不到最小的，直接返回了
        // 没有退，就会找到对应的最小顶点位置
        visited[min_index] = true;

        for (int i = 0; i < mg.vertex_count; i++) {
            // 找邻边，然后比新长
            if (mg.edges[min_index][i] != 0 and distances[0][i] > distances[0][min_index] + mg.edges[min_index][i]) 
            {   
                distances[0][i] = distances[0][min_index] + mg.edges[min_index][i];
                distances[1][i] = min_index;
                pq.push({distances[0][i], i});
            }
        }
    }

    min_distance_display(mg, start, distances);
}


// MGraph 的 Floyd 
void Floyd_Min_Distance(MGraph & mg) {
    // 前区表
    vector<vector<int>> distance(mg.vertex_count, vector<int>(mg.vertex_count));
    vector<vector<int>> pre(mg.vertex_count, vector<int> (mg.vertex_count, -1));

    // 拷贝 MGraph 的内容
    for (int i = 0; i < mg.vertex_count; i++) {
        distance[i] = vector<int> (mg.edges[i], mg.edges[i] + mg.vertex_count);
    }

    for (int k = 0; k < mg.vertex_count; k++) {
        for (int i = 0; i < mg.vertex_count; i ++) {
            for (int j = 0; j < mg.vertex_count; j++) {
                if (distance[i][j] > distance[i][k] + distance[k][j]) {
                    distance[i][j] = distance[i][k] + distance[k][j];
                    pre[i][j] = k;
                }
            }
        }
    }

    // 输出距离和前驱表
    cout << "Distance Matrix:\n";
    for (const auto &row : distance) {
        for (int d : row) {
            if (d == INFIN) cout << setw(5) << "inf" << " ";
            else cout << setw(5) << d << " ";
        }
        cout << endl;
    }

    cout << "Predecessor Matrix:\n";
    for (const auto &row : pre) {
        for (int p : row) {
            cout << setw(5) << p << " ";
        }
        cout << endl;
    }
}