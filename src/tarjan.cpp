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
bool biconnected = true;
int sub_tree_count = 0;
int articulation_point_count = 0;

vector<vector<int>> components;
stack<int> S;

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

void dfs(uint32_t u, uint32_t time, Graph graph, uint32_t dfs_dn[], uint32_t low[], uint32_t parent[])
{
    // cout << "here" << endl;
    dfs_dn[u] = low[u] = time++;
    S.push(u);

    for (uint32_t v : graph.AdjList[u])
    {
        // cout << "doing dfs" << endl;
        if (dfs_dn[v] == 0)
        {
            // cout << "here 2" << endl;
            parent[v] = u;
            dfs(v, time, graph, dfs_dn, low, parent);
            low[u] = min(low[u], low[v]);
            if (low[v] >= dfs_dn[u])
            {
                if (u != 0)
                {
                    biconnected = false;
                    articulation_point_count++;
                    // cout << "found articulation point: " << articulation_point_count << endl;
                }
                else
                {
                    sub_tree_count++;
                    if (sub_tree_count > 1)
                    {
                        biconnected = false;
                    }
                    // cout << "subtree count: " << sub_tree_count << endl;
                }
                // cout << "found articulation point" << endl;
                vector<int> component;
                uint32_t w;
                while (true)
                {
                    w = S.top();
                    S.pop();
                    component.push_back(w);
                    if (w == v)
                    {
                        break;
                    }
                }
                component.push_back(u);
                components.push_back(component);
            }
        }
        else if (v != parent[u])
        {
            low[u] = min(low[u], dfs_dn[v]);
        }
    }
}

void find_components(uint32_t n, Graph graph)
{
    uint32_t dfs_dn[n];
    uint32_t low[n];
    uint32_t parent[n];
    uint32_t time = 1;

    for (uint32_t i = 0; i < n; i++)
    {
        dfs_dn[i] = 0;
        low[i] = 0;
        parent[i] = 0;
    }

    for (uint32_t i = 0; i < n; i++)
    {
        if (dfs_dn[i] == 0)
        {
            dfs(i, time, graph, dfs_dn, low, parent);
        }
    }
}

// void build_adjlist(Graph &graph, ifstream &InputFile)
// {
//     string line;
//     uint32_t i;
//     cout << graph.n << endl;
//     for (i = 0; i < graph.n + 1; i++)
//     {
//         cout << "i: " << i << endl;
//         getline(InputFile, line);
//         cout << "line: " << line << endl;
//         istringstream iss(line);
//         vector<string> vertices;
//         string vertex;
//         if (i)
//         {
//             while (getline(iss, vertex, ' '))
//             {
//                 cout << "vertex: " << vertex << endl;
//                 vertices.push_back(vertex);
//             }
//             for (auto &v : vertices)
//             {
//                 cout << "v: " << v << endl;
//                 uint32_t j = stoi(v);
//                 cout << "j: " << j << endl;
//                 graph.AdjList[i - 1].push_back(j);
//                 graph.m++;
//             }
//         }
//         cout << "done" << endl;
//     }
//     cout << "**************************************************" << endl;
// }

void verify(Graph &graph)
{
    for (int i = 0; i < graph.n; i++)
    {
        for (auto &itr : graph.AdjList[i])
        {
            cout << itr << endl;
        }
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

    
    // build_adjlist(graph, InputFile);
    // verify(graph);
    LoadGraph(InputFile, graph);
    // PrintGraph(graph);
    find_components(graph.n, graph);

    // for (const auto &component : components)
    // {
    //     for (int v : component)
    //     {
    //         cout << v << " ";
    //     }
    //     cout << endl;
    // }

    // ofstream jenfile;
    // jenfile.open("jen_output.txt");


    if (biconnected)
    {
        cout << "BICONNECTED" << endl;
    }
    else
    {
        cout  << "NOT BICONNECTED" << endl;
    }
    // jenfile.close();
    return 0;
}