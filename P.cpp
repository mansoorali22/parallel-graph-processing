#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <queue>
#include <climits>
#include <chrono>
#include <sstream>
#include <omp.h>
#include <mpi.h>

using namespace std;
using namespace std::chrono;

struct Edge {
    int to;
    int weight;
};

vector<vector<Edge>> readGraphFromFile(const string& filename, int& n, int& m) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Unable to open input file." << endl;
        exit(1);
    }

    inputFile >> n >> m; // Read total nodes (n) and edges (m)
    vector<vector<Edge>> graph(n + 1);

    for (int i = 0; i < m; ++i) {
        int node1, node2, weight;
        inputFile >> node1 >> node2 >> weight;
        graph[node1].push_back({node2, weight});
        graph[node2].push_back({node1, weight}); // For undirected graph
    }

    inputFile.close();
    return graph;
}

vector<pair<int, int>> readNodePairsFromFile(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Unable to open input file." << endl;
        exit(1);
    }

    vector<pair<int, int>> nodePairs;
    int node1, node2;
    while (inputFile >> node1 >> node2) {
        nodePairs.push_back({node1, node2});
    }

    inputFile.close();
    return nodePairs;
}

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

void findKShortestPaths(const vector<vector<Edge>>& graph, int n, int k, int start, int goal, int world_rank, stringstream& ss) {
    const int INF = INT_MAX;
    vector<vector<int>> global_dis(n + 1, vector<int>(k, INF));
    global_dis[start][0] = 0;

    omp_set_num_threads(omp_get_max_threads());

    // Using a parallel region with each thread having its own local priority queue and distance matrix
    #pragma omp parallel
    {
        vector<vector<int>> local_dis(n + 1, vector<int>(k, INF));
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> local_pq;
        if (omp_get_thread_num() == 0) {
            local_pq.push({0, start});
            local_dis[start][0] = 0;
        }

        #pragma omp barrier // Ensure all threads are ready before proceeding

        while (!local_pq.empty()) {
            int d = local_pq.top().first;
            int u = local_pq.top().second;
            local_pq.pop();

            if (d > local_dis[u][k-1]) continue;

            for (const auto& edge : graph[u]) {
                int v = edge.to;
                int newDist = d + edge.weight;

                if (newDist < local_dis[v][k-1]) {
                    local_dis[v][k-1] = newDist;
                    sort(local_dis[v].begin(), local_dis[v].end());
                    local_pq.push({newDist, v});
                }
            }
        }

        // Merge local distances into the global distance matrix
        #pragma omp critical
        {
            for (int i = 1; i <= n; ++i) {
                for (int j = 0; j < k; ++j) {
                    if (local_dis[i][j] < global_dis[i][j]) {
                        global_dis[i][j] = local_dis[i][j];
                        sort(global_dis[i].begin(), global_dis[i].end());
                    }
                }
            }
        }
    }

    ss << "Process " << world_rank << ": Node " << start << " to Node " << goal << " K shortest paths: ";
    for (int i = 0; i < k && global_dis[goal][i] != INF; ++i) {
        ss << global_dis[goal][i] << " ";
    }
    ss << endl;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int n, m, K = 3; // Number of shortest paths to find
    vector<vector<Edge>> graph;
    //string filename = "Email-Formatted.txt"; // Email formartted Graph
    //string filename = "Enron-Formatted.txt"; // Enron- Formatted Graph
    string filename = "Doctor-Who.txt"; // Enron- Formatted Graph
    //string filename = "graph.txt";


    double start_time = 0.0, stop_time = 0.0;

    if (world_rank == 0) {
        graph = readGraphFromFile(filename, n, m);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 1; i <= n; ++i) {
        int num_edges = world_rank == 0 ? graph[i].size() : 0;
        MPI_Bcast(&num_edges, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (world_rank != 0) {
            graph.resize(n + 1);
            graph[i].resize(num_edges);
        }
        for (Edge& edge : graph[i]) {
            MPI_Bcast(&edge.to, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&edge.weight, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
    }

    vector<pair<int, int>> node_pairs;
    if (world_rank == 0) {
        node_pairs = readNodePairsFromFile("output.txt");
    }

    MPI_Barrier(MPI_COMM_WORLD); // Synchronize before starting timing
    if (world_rank == 0) {
        start_time = MPI_Wtime(); // Start the timer only once on the master before scattering
    }

    vector<pair<int, int>> local_pairs(2);  // Each process gets 2 pairs
    MPI_Scatter(node_pairs.data(), 4, MPI_INT, local_pairs.data(), 4, MPI_INT, 0, MPI_COMM_WORLD);

    stringstream ss;
    for (const auto& pair : local_pairs) {
        findKShortestPaths(graph, n, K, pair.first, pair.second, world_rank, ss);
    }

    cout << ss.str();

    MPI_Barrier(MPI_COMM_WORLD); // Ensure all processes finish before stopping the timer
    if (world_rank == 0) {
        stop_time = MPI_Wtime(); // Stop the timer
        double duration = stop_time - start_time;
        cout << "Total execution time: " << duration << " seconds." << endl;
    }

    MPI_Finalize();
    return 0;
}

