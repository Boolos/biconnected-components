# biconnected-components
Experimentation with parallelizing algorithms related to biconnected components

####Each project is now in their separate directory.
You can use the code in graph-generator directory to generate random graphs and write them out to a file. 
The supported flags are defined in the main.cpp file, but some of the more useful ones for you will be these:
* <code>-n nvertices</code> -> the number of vertices in the graph
* <code>-m nedges</code> -> the number of edges in the graph
* <code>-o file_path</code> -> write the graph to a file
* <code>-d</code> -> if specified, the graph will be a directed graph, otherwise by default it will be undirected.
* <code>-c</code> -> if specified, the graph will be connected, otherwise by default there is no guarantee that it will be connected.
* <code>-k</code> -> if specified, the graph will be a complete graph of size n (specified by the -n parameter)
So if you want to specify an average degree per vertex, you would need to make sure the value of m/n is equal to the desired average degree.