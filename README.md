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
