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

//Step 1: BFS
//Input: Graph G
//Output: a Spanning Tree T, with array P(v) stores parent of v, and array L(v) stores the level
//r, source and root, P(r) = -1, L(r) = 0
Graph Bicc::breadthFirstSearch(Graph& sparseGraph) {
    Graph tree;
	
	Vertex root = sparseGraph.getVertex(0);
	root.parent->setId(-1);
	root.color = "white";
	root.level = 0;
	
	//do BFS, modify P and L along the way.
    for(size_t i = 0; i < sparseGraph.getVertexCount(); i++){
		sparseGraph.getVertex(i).color = "white";
	}
       
    queue<Vertex> q;
	list<Vertex> neighbors = root.getNeighbors();
	root.color = "gray";
    q.push(root);                   
 
    while(!q.empty())
    {
        root = q.front();
        q.pop();
 
        for(auto& neighbor : neighbors)
        {
            if(neighbor.color == "white")
            {
                neighbor.color = "gray";
                neighbor.level = neighbor.level + 1;
				neighbor.parent->setId(neighbor.parent->getId() + 1);
                q.push(neighbor);
				tree.add(neighbor);
            }
        }
    }
	//add tree edges to T.
    return tree;
}

//Step 2: Getting Bridges
//Input: Graph G, Spanning Tree T
//Output: Vector of sub-Graphs Components  (G1, G2, ...)
//LCA(w,v): lowest node which has both w,v as descendants.
vector<Graph> Bicc::findBridges(Graph& graph, Graph& bfsTree) {
    vector<Graph> components;

	//for all e(w,v) in the G not in T, mark all non-tree edges until LCA (w,v)
	Graph G_T = graph.difference(bfsTree);
	vector<Edge> differenceGT = G_T.getEdges();
	vector<pair<Vertex, Vertex>> wv;
	vector<Edge> B;
    for(int i = differenceGT.size(); i > 0; i--){
		wv[i] = make_pair(differenceGT[i].getU(), differenceGT[i].getV());		
		if (wv[i].first.parent->getId() == wv[i].second.parent->getId()){
			differenceGT[i].isBridge = false;
			differenceGT[i].setLca(wv[i].first.parent->getId());
		}
		else {
			//tree edges not marked are the bridges
			differenceGT[i].isBridge = true;
			B.push_back(differenceGT[i]);
		}
	}

	//for all e in T that are not marked, add them to B	
	//for all e(x,y) in B, x = P(y), set P(y) = -1 to decompose G into vector of 2-edge-connected components 
	vector<pair<Vertex, Vertex>> xy;
	for(int i = 0; i < B.size(); i++) {
		xy[i] = make_pair(B[i].getU(), B[i].getV());
		
			if (xy[i].second.parent->getId() == xy[i].first.getId()) {
				xy[i].second.parent->setId(-1);
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
