#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <queue>
#include <climits>
#include <chrono> // For timing

using namespace std;
using namespace std::chrono;

struct Edge {
    int node1;
    int node2;
    int weight;
};

// Function to read edges (node pairs with weights) from graph.txt
vector<Edge> readEdgesFromFile(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Unable to open input file." << endl;
        exit(1);
    }

    int totalNodes, totalEdges;
    inputFile >> totalNodes >> totalEdges; // Read total nodes and edges
    vector<Edge> edges;

    for (int i = 0; i < totalEdges; ++i) {
        Edge edge;
        inputFile >> edge.node1 >> edge.node2 >> edge.weight;
        edges.push_back(edge);
    }

    inputFile.close();
    return edges;
}

// Function to randomly select node pairs from a vector of edges
vector<pair<int, int>> selectNodePairs(const vector<Edge>& edges, int numPairs) {
    srand(time(nullptr));
    vector<pair<int, int>> selectedPairs;

    for (int i = 0; i < numPairs; ++i) {
        int randomIndex = rand() % edges.size();
        selectedPairs.push_back({edges[randomIndex].node1, edges[randomIndex].node2});
    }

    return selectedPairs;
}

// Function to write node pairs (without weights) to output.txt
void writeNodePairsToFile(const string& filename, const vector<pair<int, int>>& nodePairs) {
    ofstream outputFile(filename);
    if (!outputFile) {
        cerr << "Error: Unable to open output file." << endl;
        exit(1);
    }

    for (const auto& pair : nodePairs) {
        outputFile << pair.first << " " << pair.second << endl;
    }

    outputFile.close();
}

// Function to find K shortest path lengths from a graph represented as adjacency list
vector<int> findKShortestPaths(const vector<vector<Edge>>& graph, int n, int k, int startNode, int targetNode) {
    const int INF = INT_MAX;
    vector<vector<int>> dis(n + 1, vector<int>(k, INF));
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    pq.push({0, startNode});
    dis[startNode][0] = 0;

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dis[u][k-1]) continue;

        for (const auto& edge : graph[u]) {
            int v = (u == edge.node1) ? edge.node2 : edge.node1;
            int newDist = d + edge.weight;

            if (newDist < dis[v][k-1]) {
                dis[v][k-1] = newDist;
                sort(dis[v].begin(), dis[v].end());

                pq.push({newDist, v});
            }
        }
    }

    return dis[targetNode];
}

// Function to print K shortest paths to a node
void printKShortestPaths(int startNode, int targetNode, const vector<int>& shortestPaths, int k) {
    cout << "K shortest paths from node " << startNode << " to node " << targetNode << ": ";
    for (int i = 0; i < k; ++i) {
        cout << shortestPaths[i] << " ";
    }
    cout << endl;
}

int main() {
    //string graphFilename = "Email-Formatted.txt"; // EU-Email Graph
    //string graphFilename = "Enron-Formatted.txt"; // Enron -Formatted Graph
    string graphFilename = "network_edges.txt";
    string outputFilename = "output.txt";
    int numPairsToSelect = 10;
    int k = 3; // Number of shortest paths to find

    // Read edges (with weights) from graph.txt
    vector<Edge> edges = readEdgesFromFile(graphFilename);

    // Select random node pairs from the read edges
    vector<pair<int, int>> selectedNodePairs = selectNodePairs(edges, numPairsToSelect);

    // Write selected node pairs (without weights) to output.txt
    writeNodePairsToFile(outputFilename, selectedNodePairs);

    // Construct graph from selected node pairs with actual weights from graph.txt
    vector<vector<Edge>> graph(edges.size() + 1);
    for (const auto& pair : selectedNodePairs) {
        int node1 = pair.first;
        int node2 = pair.second;
        
        // Find the edge weight between node1 and node2 in the original graph
        int weight = INT_MAX; // Initialize weight to a large value
        for (const auto& edge : edges) {
            if ((edge.node1 == node1 && edge.node2 == node2) || (edge.node1 == node2 && edge.node2 == node1)) {
                weight = edge.weight;
                break;
            }
        }
        
        graph[node1].push_back({node1, node2, weight});
        graph[node2].push_back({node2, node1, weight}); // Bidirectional edge
    }

    // Find and print K shortest paths for each selected node pair
    auto startTime = high_resolution_clock::now();
    for (const auto& pair : selectedNodePairs) {
        int startNode = pair.first;
        int targetNode = pair.second;

        vector<int> shortestPaths = findKShortestPaths(graph, edges.size(), k, startNode, targetNode);
        printKShortestPaths(startNode, targetNode, shortestPaths, k);
    }
    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(endTime - startTime);
    cout << "Total execution time: " << duration.count() << " ms" << endl;

    return 0;
}

