# biconnected-components
Experimentation with parallelizing algorithms related to biconnected components

The graph class (may be replaced by a better one later) assumes the file is stored with the first line being the number of vertices V, followed by V lines. For each line i, the line begins with the number of vertices adjacent to vertex i (call it x), followed by the x vertices of vertex i's adjacency list.

#####To compile and run tests for graphlib:
`g++-5 -O3 -fopenmp -m64 -std=c++11 ./graphlib/*.cpp`
