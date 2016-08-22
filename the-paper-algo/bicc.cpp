#include "bicc.hpp"
#include <queue>

using namespace std;
using namespace csce;
using std::queue;

Bicc::Bicc(int nthreads) : _nthreads(nthreads) {}

vector<Vertex> Bicc::getArticulationPoints(Graph& sparseGraph) {
    // BFS - the result of this function should be the BFS spanning tree
    // each vertex should have its parent marked as well as its level
    // each non tree edge should be marked
    auto bfsTree = this->breadthFirstSearch(sparseGraph);

    // Mark bridges on sparse graph, bridge = non-tree edge not on any cycle 
    // each non tree edge should have its least common ancestor marked 
    // each component is a subgraph of the sparse graph that has been disconnected by removing the bridges
    auto components = findBridges(sparseGraph, bfsTree);

    // Get articulation points in parallel
    auto artPoints = findArtPointsInParallel(components, bfsTree);

    return artPoints;
}

Graph Bicc::breadthFirstSearch(Graph& sparseGraph) {
    Graph bfsTree;	
	auto lev = 0;
    
	//initialize
	vector<Vertex> verticies = sparseGraph.getVerticies();
	for(int i = 0; i < verticies.size(); i++){
		verticies[i].color = "white";
		verticies[i].level = lev;
	}
	
	Vertex start = verticies[0];
    queue<Vertex> VertexQueue;
    VertexQueue.push(start);
 
    while (!VertexQueue.empty())    
    {
        Vertex current = VertexQueue.front();
        current.color = "gray"; //current vertex is being processed
		auto neighbors = current.getNeighbors();
		
		//process all neighbors of current vertex
        for(auto& n : neighbors) {
        	
            if (n.color == "white") {   // This is an unvisited vertex
                n.level = lev + 1;          // Set level
                n.parent = &current;       // Set parent
                n.color = "gray";			// Set color visited
            	continue;  	
			}
			
			bfsTree.add(current, n); //add the edge to bfsTree
            VertexQueue.push(n);    // Add it to the queue
            
        }
        VertexQueue.pop();    // Pop out the processed vertex
        lev++;  // The next level
        
    } 
    
    return bfsTree;
}

vector<Graph> Bicc::findBridges(Graph& graph, Graph& bfsTree) {

    vector<Graph> components;
	vector<Edge> graph_edges = graph.getEdges();
	Graph diffGT = graph.difference(bfsTree);
	vector<Edge> diffGT_edges = diffGT.getEdges();
	vector<pair<Vertex, Vertex>> wv;
	vector<Edge> Bridges;
	
	for(int i = 0; i < graph_edges.size(); i++) {
			graph_edges[i].isBridge = true;
	}
	/*
    for(int i = 0; i < diffGT_edges.size(); i++){
		Vertex w = diffGT_edges[i].getU();
		Vertex v = diffGT_edges[i].getV();
				
		if (w.parent->getId() != v.parent->getId()){
			continue;
			}
			
			diffGT_edges[i].isBridge = false;
			diffGT_edges[i].setLca(wv[i].first.parent->getId());
	}*/
	
	for(int i = 0; i < graph_edges.size(); i++) {
			if (graph_edges[i].isBridge == true)
			Bridges.push_back(graph_edges[i]);
	}

	vector<pair<Vertex, Vertex>> xy;
	for(int i = 0; i < Bridges.size(); i++) {
		xy.push_back(make_pair(Bridges[i].getU(), Bridges[i].getV()));
		
			if (xy[i].second.parent->getId() == xy[i].first.getId()) {
				continue;
				}
				
				Vertex parent(-1);
				xy[i].second.parent = &parent; 
				
				components[i].add(xy[i].second);
				list<Vertex> neighbors = xy[i].second.getNeighbors();
				
				for(auto& neighbor : neighbors){				
					if(neighbor.parent->getId() == xy[i].second.getId()){
						continue;
					}
				components[i].add(neighbor);
			}
		}

    return components;
}

vector<Vertex> Bicc::findArtPointsInParallel(vector<Graph> components, Graph& bfsTree) {
    vector<Vertex> artPoints;

    #pragma omp parallel for num_threads(_nthreads)
    for (int i = 0; i < components.size(); i++) {
        auto auxGraph = constructAuxGraph(components[i]);
        auto auxComponents = findBridges(auxGraph, bfsTree);
        for (auto auxComp = auxComponents.begin(); auxComp != auxComponents.end(); auxComp++) {
            if (this->isArticulationPoint(*auxComp, bfsTree)) {
                artPoints.push_back(bfsTree.getRoot());
            }
        }
    }

    return artPoints;
}

Graph Bicc::constructAuxGraph(Graph graph) {
    auto edges = graph.getEdges();
    for (auto edge = edges.begin(); edge != edges.end(); edge++) {
        auto lca = edge->getLca();
        Vertex alias(lca.getId());
        alias.isAlias = true;
        auto neighbors = lca.getNeighbors();
        for (auto n = neighbors.begin(); n != neighbors.end(); n++) {
            if (n->parent->getId() != lca.getId()) {
                continue;
            }
            graph.remove(lca, *n);
            graph.add(alias, *n);
        }
        graph.add(lca, alias);
    }
    return graph;
}

bool Bicc::isArticulationPoint(const Graph& auxGraph, const Graph& bfs) const {
    auto root = bfs.getRoot();
    auto auxRoot = auxGraph.getVertex(root);
    auto edgesFromRoot = auxGraph.getEdges(root);

    return auxRoot.getDegree() > 1 && edgesFromRoot.size() == 1;
}
