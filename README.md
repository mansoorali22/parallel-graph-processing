# Parallel Execution for Graph Processing

This repository contains the source code and documentation for our Parallel and Distributed Computing (PDC) course project. The goal of this project was to evaluate the performance of parallel execution techniques for computing the **K shortest paths** in graph datasets using **MPI (Message Passing Interface)** and **OpenMP (Open Multi-Processing)**.

## 🧠 Project Objective

To implement and compare the performance of **sequential vs. parallel graph processing** approaches using:
- **MPI** for distributed computing
- **OpenMP** for shared-memory parallelism
- A hybrid MPI + OpenMP model in C++


## ⚙️ Experimental Setup

- **Graph Datasets:**
  1. Email-Formatted Graph
  2. Enron-Formatted Graph
  3. Doctor Who Graph

- **Tools and Technologies:**
  - C++ (with `mpicxx` and OpenMP)
  - MPI (OpenMPI)
  - OpenMP

- **Hardware Used:**
  - Processor: Intel Core i5 (7th Gen)
  - RAM: 8 GB
  - Nodes: 4 (for MPI)

## 🛠️ Implementation

### 🧮 Algorithm
- A **Dijkstra-like** algorithm was used to compute **K shortest paths**.
- **MPI** handled inter-process communication and distributed workloads.
- **OpenMP** accelerated computations inside each MPI process using multithreading.

### 📂 Code Structure
- `P.cpp`: Parallel implementation using MPI + OpenMP
- `S.cpp`: Sequential implementation using standard C++

## ▶️ Running the Code

### 🔧 Prerequisites

Make sure the following are installed on your system:
- [OpenMPI](https://www.open-mpi.org/)
- C++ compiler supporting OpenMP and MPI (e.g., `g++`, `mpicxx`)

### 💻 Compilation & Execution

#### 🔀 Parallel Version (P.cpp)

```bash
# Compile the parallel version
mpicxx -fopenmp -o p1 P.cpp

# Run with 4 MPI processes (you can adjust the number)
mpirun -np 4 ./p1
```

#### ⏳ Sequential Version (S.cpp)

```bash
# Compile the sequential version
g++ -o s1 S.cpp

# Run the sequential executable
./s1
```

## 🚀 Experimental Results

| Dataset          | Execution Type | Time Taken       |
|------------------|----------------|------------------|
| Email-Formatted  | Sequential     | 2217 ms          |
|                  | Parallel       | 12.15 seconds    |
| Enron-Formatted  | Sequential     | 2060 ms          |
|                  | Parallel       | 2.14 seconds     |
| Doctor Who       | Sequential     | 91 ms            |
|                  | Parallel       | 0.347 seconds    |

### 📈 Speedup Formula

```
Speedup = Sequential Time / Parallel Time
```

## 🧩 Key Concepts

### MPI Usage
- `MPI_Bcast`: Broadcasting graph data to all nodes.
- `MPI_Scatter`: Distributing node pairs to different processes.
- `MPI_Barrier`: Synchronization point for all processes.
- `MPI_Wtime`: Measuring execution time.

### OpenMP Usage
- `omp_set_num_threads`: Set maximum threads.
- `#pragma omp parallel`: Start of parallel region.
- `#pragma omp critical`: Synchronize shared resource access.

## 🧪 Testing and Optimization

### Challenges
- Preprocessing node labels into numeric format.
- Ensuring consistent outputs in parallel execution.
- Load balancing and avoiding race conditions.

### Optimization Techniques
- Efficient mapping of node labels using hash maps.
- Independent data structures in threads to avoid conflicts.
- Critical sections for merging results safely.

## 📊 Graphical Visualizations
- Bar and line graphs to visualize execution time and speedup.

## 🧠 Insights & Discussion

- **Scalability:** Parallel implementation showed significant speedups for larger graphs.
- **Accuracy:** Minor variations in floating-point precision and thread timing can lead to output differences between sequential and parallel versions.
- **Performance Bottlenecks:** Communication overhead and thread synchronization required careful tuning.

## ✅ Conclusion

This project demonstrated the significant performance benefits of using parallel programming models like **MPI** and **OpenMP** for large-scale graph processing tasks. The hybrid approach allowed both distributed and shared-memory computations, achieving efficiency gains while highlighting practical challenges like synchronization, correctness, and scalability.

---
