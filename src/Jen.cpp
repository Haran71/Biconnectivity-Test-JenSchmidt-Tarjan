#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <util.h>

using namespace std;

struct Graph
{
    vector<vector<uint32_t>> AdjList;
    uint32_t n;
    uint32_t m;
    Graph(uint32_t n) : n(n), m(0)
    {
        AdjList.resize(n);
    }
};


vector<bool> visited; // array to keep track of visited vertices
vector<int> parent; // array to keep track of the parent of each vertex in the DFS tree
vector<vector<int>> dfs_tree; // adjacency list of the DFS tree
vector<pair<int, int>> back_edges; // list of backedges
vector<vector<int>> ears; // to store the ears
set<int> visited_nodes; // to keep track of visited nodes while forming the ears

vector<int> discovery_time;
int time_counter = 0;

auto cmp = [](std::pair<int, int> p1, std::pair<int, int> p2) {
    return discovery_time[p1.first] < discovery_time[p2.first];
};

void SkipNLines(std::ifstream &InputFile, uint64_t n, std::ios::seekdir whence = std::ios::cur,
                std::streamoff offset = 0)
{
    InputFile.seekg(offset, whence);
    std::string line;
    for (uint64_t i = 0; i < n; ++i)
    {
        getline(InputFile, line);
    }
}

void PrintGraph(const Graph &graph, bool MatchInputFormat = false)
{
    if (!MatchInputFormat)
    {
        cout << "GRAPH\n";
        cout << "Number of vertices = " << graph.n << "\n";
        cout << "Number of edges = " << graph.m << "\n";
    }
    for (uint32_t i = 0; i < graph.n; ++i)
    {
        if (!MatchInputFormat)
            cout << i << " | ";
        for (uint32_t j = 0; j < graph.AdjList[i].size(); ++j)
        {
            cout << graph.AdjList[i][j];
            if (j < graph.AdjList[i].size() - 1)
            {
                cout << " ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

void LoadGraph(std::ifstream &InputFile, Graph &graph)
{
    SkipNLines(InputFile, 1, std::ios::beg);
    std::string line;
    line.reserve(graph.n * 2);

    uint32_t i = 0;
    while (std::getline(InputFile, line))
    {
        std::stringstream stream(line);
        uint32_t j = 0;
        while (stream >> j)
        {
            // to make the graph undirected
            if(find(begin(graph.AdjList.at(i)), end(graph.AdjList.at(i)), j) == graph.AdjList.at(i).end() && i != j){
                graph.AdjList.at(i).push_back(j);
                graph.AdjList.at(j).push_back(i);
                ++graph.m;
            } 
        }
        ++i;
    }

    assert(graph.AdjList.size() == graph.n);
}

bool checkForBiconnectivity(int total_edges){
    // First condition: If all the edges of the graph are visited in the decomposition
    // To check for this we simply count the number of edges in the decomposition

    int edge_count = 0;
    for (auto b: ears){
        edge_count += b.size() - 1;
    }

    if(edge_count != total_edges) return false;

    // Second condition: If any cycle apart from the first chain exists.
    // Now in our case, we have split the first chain into P0, and P1.
    // So our chain decomposition should contain zero cycles

    for (auto b: ears){
        if(b[0] == b[b.size()-1]) return false;
    }

    return true;

}

void findEars(){

    if (back_edges.size() > 0) {
        vector<int> P0;
        P0.push_back(back_edges[0].first);
        P0.push_back(back_edges[0].second);

        ears.push_back(P0);
    }

    for (auto b: back_edges){
        vector<int> ear;
        int u = b.first;
        int v = b.second;

        ear.push_back(u);
        ear.push_back(v);

        visited_nodes.insert(u);

        if(visited_nodes.count(v) != 0){
            ears.push_back(ear);
            continue;
        }

        visited_nodes.insert(v);

        int next_node = dfs_tree[v][0];

        while(visited_nodes.count(next_node) == 0){
            ear.push_back(next_node);
            visited_nodes.insert(next_node);
            next_node = dfs_tree[next_node][0];
        }
        ear.push_back(next_node);

        if(b == back_edges[0]){
            ear.erase(ear.begin());
        }

        ears.push_back(ear);
    } 
}

void dfs(int u, Graph g) {
    visited[u] = true;
    time_counter++;
    discovery_time[u] = time_counter;
    for (int v : g.AdjList[u]) {
        if (!visited[v]) {
            parent[v] = u;
            dfs_tree[v].push_back(u);
            dfs(v,g);
        }
        else if (v != parent[u]) {
            pair<int,int> t = {u,v};
            if(find(begin(back_edges), end(back_edges), t) == back_edges.end()){
                back_edges.push_back({v, u});
            }
            
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Incorrect input" << endl;
        return 1;
    }
    ifstream InputFile(argv[1]);
    if (!InputFile)
    {
        cerr << "Error in opening file" << endl;
        return 1;
    }

    uint32_t n;

    InputFile >> n;
    Graph graph(n);

    visited.resize(n+1, false);
    parent.resize(n+1, -1);
    dfs_tree.resize(n+1);
    discovery_time.resize(n+1, -1);

    LoadGraph(InputFile, graph);
    // PrintGraph(graph);

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            if(i>0){
                cout << "NOT BICONNECTED" << endl;
                exit(0);
            }
            dfs(i,graph);
        }
    }

    // print the adjacency list of the DFS tree
    // for (int i = 0; i <n; i++) {
    //     cout << i << ": ";
    //     for (int v : dfs_tree[i]) {
    //         cout << v << " ";
    //     }
    //     cout << endl;
    // }

    // cout << "Backedges: ";
    // for (auto e : back_edges) {
    //     cout << "(" << e.first << "," << e.second << ") ";
    // }
    // cout << endl;

    sort(begin(back_edges), end(back_edges), cmp);

    findEars();

    // cout << "Ears:" << endl;
    // for(int i = 0; i < ears.size(); i++){
    //     for (auto e : ears[i]){
    //         cout << e << " ";
    //     }
    //     cout << endl;
    // }
    if(checkForBiconnectivity(graph.m)){
        cout << "BICONNECTED" << endl;
    }
    else{
        cout << "NOT BICONNECTED" << endl;
    }
    return 0;
}