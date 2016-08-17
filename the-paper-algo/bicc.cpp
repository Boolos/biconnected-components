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

    // Mark bridges on sparse graph 
    // each non tree edge should have its least common ancestor marked 
    // each component is a subgraph of the sparse graph that has been disconnected by removing the bridges
    auto components = findBridges(sparseGraph, bfsTree);

    // Get articulation points in parallel
    auto artPoints = findArtPointsInParallel(components, bfsTree);

    return artPoints;
}

Graph Bicc::breadthFirstSearch(Graph& sparseGraph) {
    Graph bfsTree;
	
	//build adjacency list from sparseGraph
	vector<Vertex> verticies = sparseGraph.getVerticies();
	for(int i = 0; i < verticies.size(); i++){
		bfsTree.add(verticies[i]);
		verticies[i].color = "white";
	}
	
    int lev;
    lev = 0;
	Vertex start = bfsTree.getRoot();
    start.level = lev;
    list<Vertex> VertexQueue;
    VertexQueue.push_back(start);
 
    while (!VertexQueue.empty())    
    {
        Vertex current = VertexQueue.front();
		list<Vertex> neighbors = current.getNeighbors();
		
		//process all neighbors of current vertex
        for(auto neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++) {
            if (neighbor->color == "white") {            // This is an unvisited vertex
                neighbor->level = lev + 1;          // Set level
                neighbor->parent = &current;       // Set parent
                neighbor->color = "gray";
				VertexQueue.push_back(*neighbor);    // Add it to the queue
			}
				
				//if neighbor is gray, back edge(non tree), remove from bfstree, color black
				else if (neighbor->color == "gray"){
					bfsTree.remove(current, *neighbor);
					neighbor->color = "black";
				}	
        }
        VertexQueue.pop_front();    // Pop out the processed vertex
        ++lev;  // The next level
    }
    return bfsTree;
}

vector<Graph> Bicc::findBridges(Graph& graph, Graph& bfsTree) {
    vector<Graph> components;

	Graph G_T = graph.difference(bfsTree);
	vector<Edge> differenceGT = G_T.getEdges();
	vector<pair<Vertex, Vertex>> wv;
	vector<Edge> B;
    for(int i = differenceGT.size(); i = 0; i--){
		wv.push_back(make_pair(differenceGT[i].getU(), differenceGT[i].getV()));		
		if (wv[i].first.parent->getId() == wv[i].second.parent->getId()){
			differenceGT[i].isBridge = false;
			differenceGT[i].setLca(wv[i].first.parent->getId());
		}
		else {
			
			differenceGT[i].isBridge = true;
			B.push_back(differenceGT[i]);
		}
	}

	vector<pair<Vertex, Vertex>> xy;
	for(int i = 0; i < B.size(); i++) {
		xy.push_back(make_pair(B[i].getU(), B[i].getV()));
		
			if (xy[i].second.parent->getId() == xy[i].first.getId()) {
				Vertex parent(-1);
				xy[i].second.parent = &parent; 
				components[i].add(xy[i].second);
				list<Vertex> neighbors = xy[i].second.getNeighbors();
				
				for(auto& neighbor : neighbors){				
					if(neighbor.parent->getId() == xy[i].second.getId()){
						components[i].add(neighbor);
					}
				}
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
