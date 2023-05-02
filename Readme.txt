Graph hashing and isomorphism.

A means of identifying a graph with an MD5 hash is given.
This technique is also used in a graph isomorphism algorithm.
Graphs are isomorphic if they are topologically identical.

References:

T. E. Portegys. "Graph isomorphism testing boosted by path coloring", May 2016. http://arxiv.org/abs/1606.00001
T. E. Portegys. "General Graph Identification By Hashing", Feb. 2008. http://arxiv.org/abs/1512.07263
L. P. Cordella, P. Foggia, C. Sansone, M. Vento. "Performance Evaluation
of the VF Graph Matching Algorithm", Proc. of the 10th ICIAP,
IEEE Computer Society Press, vol. 2, pp. 1038-1041, 1999.
J. R. Ullmann. "An Algorithm for Subgraph Isomorphism", 
Journal of the Assoc. for Computing Machinery, 23, pp. 31-42, 1976.
C. Sayers and A.H. Karp, "RDF Graph Digest Techniques and Potential
Applications", Mobile and Media Systems Laboratory,
HP Laboratories Palo Alto, HPL-2004-95, May 28, 2004
V. Arvind, J. Köbler, G. Rattan, O. Verbitsky. "Graph Isomorphism, Color Refinement, and Compactness", 2015

Required packages:

The UNIX version requires the gcc compiler, make command, and
the bash shell. The Windows version requires the Microsoft Visual
C++ 2013 (or later) IDE.

To build:

UNIX: type 'make'
Windows: use VC++ solution and project files.
Note: Define the THREADS compilation symbol to use parallel hashing
with pthreads.

Commands:

1. graph_hash - create graph and hash.

Usage:
    Create graph and hash:
        -numVertices <number of vertices>
        -numEdges <number of edges>
        [-labelVertices <none (default) | unique | random>]
        [-labelEdges <none (default) | unique | random>]
        [-directed (directed graph)]
        [-structureRandomSeed <graph structure random seed>]
        [-labelRandomSeed <label random seed>]
        [-save <graph save file>]
        [-verbose]
        [-numThreads <number of threads (default=1)>]
    Load graph and create hash:
        [-load <graph load file>]
        [-verbose]
        [-numThreads <number of threads (default=1)>]
        
Notes: 

See graphHashDriver.cpp as example of how to create a graph.

2. graph_isomorph_tester - graph isomorphism tests.

Usage:
    Generate graphs:
            -tests <number of graph tests>
            -numVertices <number of vertices>
            -numEdges <number of edges>
            [-directed (directed graph)]
            [-structureRandomSeed <graph structure random seed>]
            [-isomorphic (generate isomorphic graphs)]
            [-numThreads <number of threads (default=1)>]
    Load graphs:
            -load1 <graph1 load file> -load2 <graph2 load file>
            [-numThreads <number of threads (default=1)>]

3. graph_hash_special_tester - special test cases.

Usage:
        graphHashBug1Tester "graph1" | "graph2"
|       graphHashBug2Tester "graph1" | "graph2"
|       graphHashRegularGraphTester
|       graphHashCaseTester
|       graph_ge <random seed> <number of vertices> <vertex degree>
