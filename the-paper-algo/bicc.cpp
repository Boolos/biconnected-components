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
    
    //testing - debugging
	cout << "before= " << sparseGraph.getVertex(0).color << " " << sparseGraph.getVertex(0).level << endl;
	//sparseGraph.updateVertex(0, "black");
	//sparseGraph.updateVertex(0, 2);
	//cout << "after= " << sparseGraph.getVertex(0).color << " " << sparseGraph.getVertex(0).level << endl;
	
	//Vertex start = sparseGraph.getVertex(0);
    queue<Vertex> VertexQueue;
    VertexQueue.push(sparseGraph.getVertex(0));
 
    while (!VertexQueue.empty())    
    {
        //Vertex current = VertexQueue.front();
        //cout << "start queue" << endl;
        //cout << VertexQueue.front().color << endl;
        sparseGraph.updateVertex(VertexQueue.front().getId(), "gray"); //current vertex is being processed
		auto neighbors = VertexQueue.front().getNeighbors();
		//const_cast <list<Vertex>&> (neighbors);
		cout << VertexQueue.front().color << endl;
		//process all neighbors of current vertex
        for(auto n : neighbors) {
        	//cout << n.color << endl;
        	//cout << n.color << endl;
            if (n.color == "white") {   // This is an unvisited vertex   	
				cout << "visiting graph" << endl;
				sparseGraph.updateVertex(n.getId(), n.level + 1);          // Set level
				sparseGraph.updateVertex(n.getId(), VertexQueue.front());       // Set parent
				sparseGraph.updateVertex(n.getId(), "gray");			// Set color visited
				bfsTree.add(VertexQueue.front().getId(), n.getId()); //add the edge to bfsTree
				VertexQueue.push(n);    // Add it to the queue
				cout << "added to bfsTree" << endl;
				//cout << n.color << endl;
				//cout << n.level << endl;			
            }
        }
		//cout << bfsTree.getVertexCount() << endl;
        VertexQueue.pop();    // Pop out the processed vertex
    } 
    return bfsTree;
}

vector<Graph> Bicc::findBridges(Graph& graph, Graph& bfsTree) {

    vector<Graph> components;
	vector<Edge> Bridges;
	
	/*
	cout << "isBridge before= " << graph_edges[0].isBridge << endl;
	graph.updateEdge(graph_edges[0].getU().getId(), graph_edges[0].getV().getId(), false);
	cout << "isBridge after= " << graph_edges[0].isBridge << endl;
	*/
	
	//compute lca, mark non-bridges
	vector<Edge> graph_edges = graph.getEdges();
    for(int i = 0; i < graph_edges.size(); i++){
		Vertex w = graph_edges[i].getU();
		Vertex v = graph_edges[i].getV();
		
		if (bfsTree.contains(w,v) == false){
			if (w.level > v.level){
				graph.updateEdge(w.getId(), v.getId(), false);
				graph_edges[i].setLca(w.parent->getId());
			}
		
			else {
				graph.updateEdge(w.getId(), v.getId(), false);
				graph_edges[i].setLca(v.parent->getId());
			}
		}
	}
			
	//saving all bridges
	for(int i = 0; i < graph_edges.size(); i++) {
			if (graph_edges[i].isBridge == true)
			Bridges.push_back(graph_edges[i]);
	}
	
	//adding components by removing bridges
	for(int i = 0; i < Bridges.size(); i++) {
		Vertex x = Bridges[i].getU(); 
		Vertex y = Bridges[i].getV();
		
			if (y.parent == &x) {
				components[i].add(y.getId());
				list<Vertex> neighbors = y.getNeighbors();
				
				for(auto& neighbor : neighbors){				
					if(neighbor.parent == &y){
						components[i].add(neighbor.getId());
					}
				}
			}
		}
	//testing - debugging
	cout << "bridges size = " << Bridges.size() << endl;
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
	//testing - debugging
	cout << "artPoints size = " << artPoints.size() << endl;
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
