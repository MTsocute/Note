#include "../include/algraph.h"

// 辅助性函数声明
void init_ALGraph(ALGraph *alg);

static ArcNode *create_arc_node(int destination, int weight);

void add_ALGraph_edge(ALGraph *alg, int src, int dest, int weight);

void display_ALGraph(ALGraph *alg);

static void min_distance_display(const ALGraph &alg, const int &start_vertex, const vector<vector<int> > &distances);

void BFS_ALGraph(ALGraph &alg, const int &start_vertex, vector<bool> &visited, queue<int> &q);

vector<int> calculate_indegree(const ALGraph &alg);

// 初始化邻接表图
void init_ALGraph(ALGraph *alg) {
    for (int i = 0; i < MAX_SIZE; i++) {
        alg->adj_list[i].first_edge = nullptr;
        alg->adj_list[i].data = '\0'; // 初始化顶点数据为空
    }
    alg->vertex_count = 0;
    alg->edge_count = 0;
}

// 设置邻接表图的顶点
void set_ALGraph_vertices(ALGraph *alg, const string &vertex_list) {
    for (int i = 0; i < vertex_list.size(); i++) {
        alg->adj_list[i].data = vertex_list[i];
    }
    alg->vertex_count = vertex_list.size();
}

// 创建邻接表中的边节点
static ArcNode *create_arc_node(int destination, int weight) {
    ArcNode *new_node = new ArcNode;
    new_node->adjacent_vertex_index = destination;
    new_node->weight = weight; // 添加权重
    new_node->next = nullptr;
    return new_node;
}

// 为邻接表图添加边（无向），考虑权重
void add_ALGraph_edge(ALGraph *alg, int src, int dest, int weight) {
    ArcNode *new_edge1 = create_arc_node(dest, weight); // 创建新的边节点
    ArcNode *new_edge2 = create_arc_node(src, weight); // 无向图，双向边

    if (alg->adj_list[src].first_edge == nullptr) {
        alg->adj_list[src].first_edge = new_edge1;
        alg->adj_list[dest].first_edge = new_edge2;
    } else {
        // 头插新节点
        ArcNode *temp1 = alg->adj_list[src].first_edge;
        ArcNode *temp2 = alg->adj_list[dest].first_edge;

        alg->adj_list[src].first_edge = new_edge1;
        alg->adj_list[dest].first_edge = new_edge2;

        new_edge1->next = temp1;
        new_edge2->next = temp2;
    }
}

// 为邻接表图添加边（有向）
void add_ALGraph_edge_directed(ALGraph *alg, int src, int dest, int weight) {
    ArcNode *new_edge = create_arc_node(dest, weight); // 创建新的边节点
    if (alg->adj_list[src].first_edge == nullptr) {
        alg->adj_list[src].first_edge = new_edge;
    } else {
        // 头插新节点
        ArcNode *temp = alg->adj_list[src].first_edge;
        alg->adj_list[src].first_edge = new_edge;
        new_edge->next = temp;
    }
}

// 邻接表的初始化（有向图），考虑权重
ALGraph initialize_adjacency_list_graph_directed(const string &vertex_list,
                                                 const vector<tuple<int, int, int> > &edges) {
    ALGraph graph;
    init_ALGraph(&graph); // 初始化邻接表结构
    set_ALGraph_vertices(&graph, vertex_list); // 设置顶点列表

    // 添加边
    for (const auto &edge: edges) {
        int src, dest, weight;
        tie(src, dest, weight) = edge;
        add_ALGraph_edge_directed(&graph, src, dest, weight);
    }

    return graph;
}

// 邻接表的初始化（无向图），考虑权重
ALGraph initialize_adjacency_list_graph(const string &vertex_list, const vector<tuple<int, int, int> > &edges) {
    ALGraph graph;
    init_ALGraph(&graph); // 初始化邻接表结构
    set_ALGraph_vertices(&graph, vertex_list); // 设置顶点列表

    // 添加边
    for (const auto &edge: edges) {
        int src, dest, weight;
        tie(src, dest, weight) = edge;
        add_ALGraph_edge(&graph, src, dest, weight); // 使用无向图的添加函数
    }

    return graph;
}

// 邻接表的深度优先搜索
void DFS_ALGraph(ALGraph *alg, int start_vertex, vector<bool> &visited) {
    visited[start_vertex] = true;
    cout << alg->adj_list[start_vertex].data << " ";

    ArcNode *edge = alg->adj_list[start_vertex].first_edge;
    while (edge != nullptr) {
        if (!visited[edge->adjacent_vertex_index]) {
            DFS_ALGraph(alg, edge->adjacent_vertex_index, visited);
        }
        edge = edge->next;
    }
}

// BFS 的统一接口（邻接表）
void BFS(ALGraph &alg, int start_vertex) {
    vector<bool> visited(alg.vertex_count, false);
    queue<int> q;

    BFS_ALGraph(alg, start_vertex, visited, q);
    cout << endl;
}

// 邻接表的广度优先搜索
void BFS_ALGraph(ALGraph &alg, const int &start_vertex, vector<bool> &visited, queue<int> &q) {
    visited[start_vertex] = true;
    q.emplace(start_vertex);
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        cout << alg.adj_list[cur].data << " ";

        ArcNode *edge = alg.adj_list[cur].first_edge;
        while (edge != nullptr) {
            if (!visited[edge->adjacent_vertex_index]) {
                q.push(edge->adjacent_vertex_index);
                visited[edge->adjacent_vertex_index] = true;
            }
            edge = edge->next;
        }
    }
}

// 显示邻接表
void display_ALGraph(ALGraph *alg) {
    cout << "Adjacency List:" << endl;
    for (int i = 0; i < alg->vertex_count; i++) {
        cout << alg->adj_list[i].data << " -> ";
        ArcNode *edge = alg->adj_list[i].first_edge;
        while (edge != nullptr) {
            cout << alg->adj_list[edge->adjacent_vertex_index].data << "(" << edge->weight << ") ";
            edge = edge->next;
        }
        cout << endl;
    }
}

// 深度优先搜索接口（邻接表）
void DFS(ALGraph &alg, int start_vertex) {
    vector<bool> visited(alg.vertex_count, false);
    DFS_ALGraph(&alg, start_vertex, visited);
    cout << endl;
}

// 辅助输出最短路径内容
static void min_distance_display(const ALGraph &alg, const int &start_vertex, const vector<vector<int> > &distances) {
    cout << "Begin With Index: " << start_vertex << endl;
    cout << "TO : ";
    for (int i = 0; i < alg.vertex_count; i++)
        cout << setw(5) << i << " ";
    cout << endl;

    // 输出 distance 里面的数据
    cout << "Dis: ";
    for (int i = 0; i < alg.vertex_count; i++) {
        if (distances[0][i] == INFIN)
            cout << setw(5) << "inf" << " "; // 用一个空格调整宽度
        else
            cout << setw(5) << distances[0][i] << " ";
    }
    cout << endl;

    cout << "Pre: ";
    for (int i = 0; i < alg.vertex_count; i++)
        cout << setw(5) << distances[1][i] << " ";
    cout << endl;
}

// ALGraph 的 Dijistra
void Dijkstra_Min_Distance(ALGraph &alg, const int start) {
    // 三个表初始化
    vector<bool> visited(alg.vertex_count, false);
    vector<vector<int> > distances(2, vector<int>(alg.vertex_count, INFIN));
    distances[1] = vector<int>(alg.vertex_count, -1); // 前驱初始化为 -1

    distances[0][start] = 0; // 起点到自己的距离为 0
    // 优先队列，存储（距离，顶点索引）
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    pq.push({0, start}); // 初始将起点加入队列

    while (!pq.empty()) {
        // 找最小操作简化成 top
        auto [min_distances, min_index] = pq.top();
        pq.pop();

        // 防止回头遍历已经确定最小路径的情况
        if (visited[min_index]) continue;
        visited[min_index] = true;

        auto edge = alg.adj_list[min_index].first_edge;
        while (edge != nullptr) {
            int neighbor = edge->adjacent_vertex_index;
            if (!visited[neighbor] &&
                distances[0][min_index] + edge->weight < distances[0][neighbor]) {
                distances[0][neighbor] = distances[0][min_index] + edge->weight;
                distances[1][neighbor] = min_index; // 更新前驱
                // 把哪些更新了的塞入 pq
                pq.push({distances[0][neighbor], neighbor});
            }
            edge = edge->next;
        }
    }
    // 输出距离和前驱
    min_distance_display(alg, start, distances);
}

// ALGraph 的 BFS mini distance
void BFS_Min_Distance(ALGraph &alg, const int start_vertex) {
    vector<bool> visited(alg.vertex_count, false);
    queue<int> q;

    vector<vector<int> > distances(2, vector<int>(alg.vertex_count, INFIN));
    for (int i = 0; i < alg.vertex_count; i++)
        distances[1][i] = -1;

    distances[0][start_vertex] = 0; // 自己访问自己，之间的距离自然是 0
    visited[start_vertex] = true;
    q.push(start_vertex);

    while (!q.empty()) {
        int cur = q.front(); // 从哪里开始找邻边的源头
        q.pop();
        auto p_node = alg.adj_list[cur].first_edge;

        // 找到源头节点的邻接节点，记录前节点，并记录距离
        while (p_node != nullptr) {
            int adjance_index = p_node->adjacent_vertex_index;
            if (visited[adjance_index] != true) {
                q.push(adjance_index);
                visited[adjance_index] = true;
                // 记录邻边到源头的距离
                distances[0][adjance_index] = distances[0][cur] + 1;
                distances[1][adjance_index] = cur; // 记录源头
            }
            p_node = p_node->next;
        }
    }

    // 输出内容
    min_distance_display(alg, start_vertex, distances);
}

// ALGraph 的 Prim Mini Tree
void MST_Prim(const ALGraph &alg, const int start) {
    // 1. 第一层记录权值
    // 2. 第二层记录父亲节点的 index
    vector<vector<int> > dis(2, vector<int>(alg.vertex_count, INFIN));
    vector<bool> visited(alg.vertex_count, false); // 找到的新的节点是不是已经加入了树
    dis[1] = vector<int>(alg.vertex_count, -1);
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;

    // 自己到自己 0
    dis[0][start] = 0;
    pq.push({0, start}); // 我们默认从 0 顶点开始


    while (!pq.empty()) {
        auto [low_cost, low_cost_index] = pq.top();
        pq.pop();

        // 防止加入树的节点
        if (visited[low_cost_index]) continue;
        visited[low_cost_index] = true;

        auto cur_p = alg.adj_list[low_cost_index].first_edge;

        // 更新邻边，并加入到最小堆中
        while (cur_p != nullptr) {
            int neighbor = cur_p->adjacent_vertex_index;
            if (!visited[neighbor] and dis[0][neighbor] > cur_p->weight) {
                dis[0][neighbor] = cur_p->weight;
                dis[1][neighbor] = low_cost_index;
                pq.push({dis[0][neighbor], neighbor});
            }
            cur_p = cur_p->next;
        }
    }

    cout << "Mini-Spaining Tree: \n";
    for (int i = 1; i < alg.vertex_count; i++) {
        // 从1开始，0是起点
        if (dis[1][i] != -1) {
            // 只输出有父节点的
            cout << "Edge: " << dis[1][i] << " -- " << i
                    << " with weight " << dis[0][i] << endl;
        }
    }
    min_distance_display(alg, start, dis);
}

// ALGraph 的 Floyd mini distance
void Floyd_Min_Distance(ALGraph &alg) {
    vector<vector<int> > distance(alg.vertex_count, vector<int>(alg.vertex_count, INFIN));
    vector<vector<int> > pre(alg.vertex_count, vector<int>(alg.vertex_count, -1));

    // 转换 Alg 为矩阵
    for (int i = 0; i < alg.vertex_count; i++) {
        auto cur = alg.adj_list[i].first_edge;
        distance[i][i] = 0;
        while (cur != nullptr) {
            distance[i][cur->adjacent_vertex_index] = cur->weight;
            cur = cur->next;
        }
    }

    for (int k = 0; k < alg.vertex_count; k++) {
        for (int i = 0; i < alg.vertex_count; i++) {
            for (int j = 0; j < alg.vertex_count; j++) {
                if (distance[i][j] > distance[i][k] + distance[k][j]) {
                    distance[i][j] = distance[i][k] + distance[k][j];
                    pre[i][j] = k;
                }
            }
        }
    }


    // 输出距离和前驱表
    cout << "Distance Matrix:\n";
    for (const auto &row: distance) {
        for (int d: row) {
            if (d == INFIN) cout << setw(5) << "inf" << " ";
            else cout << setw(5) << d << " ";
        }
        cout << endl;
    }

    cout << "Predecessor Matrix:\n";
    for (const auto &row: pre) {
        for (int p: row) {
            cout << setw(5) << p << " ";
        }
        cout << endl;
    }
}

// 入度数组
vector<int> calculate_indegree(const ALGraph &alg) {
    vector<int> indegree(alg.vertex_count, 0);

    // 遍历邻接表，计算每个顶点的入度
    for (int i = 0; i < alg.vertex_count; i++) {
        auto edge = alg.adj_list[i].first_edge;
        while (edge != nullptr) {
            indegree[edge->adjacent_vertex_index]++; // 增加相邻顶点的入度
            edge = edge->next;
        }
    }

    return indegree;
}

// 拓扑排序
void TopoSort(const ALGraph &alg) {
    vector<int> indegree = calculate_indegree(alg); // 计算入度
    queue<int> q;
    vector<char> topo_order; // 存储拓扑排序结果
    int count  = 0; // 防止回路

    // 将所有入度为 0 的顶点入队
    for (int i = 0; i < alg.vertex_count; i++)
        if (indegree[i] == 0) q.push(i);

    // 读取所有的 0 入度节点，并且删除链接的点
    while (!q.empty()) {
        int cur_index = q.front(); q.pop();
        topo_order.push_back(alg.adj_list[cur_index].data);
        count ++;
        auto cur_node = alg.adj_list[cur_index].first_edge;
        while (cur_node != nullptr) {
            int neightbor = cur_node->adjacent_vertex_index;
            indegree[neightbor]--;
            if (indegree[neightbor] == 0) q.push(neightbor);
            cur_node = cur_node->next;
        }
    }

    if (count < alg.vertex_count) std::cout << "Looping\n";
    else {
        cout << "Topo Sort Result: \n";
        for (const auto & topo: topo_order)
            cout << topo << " ";
        cout << endl;
    }
}
