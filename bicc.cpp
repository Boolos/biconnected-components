#include "bicc.hpp"

using namespace std;
using namespace csce;

Bicc::Bicc(int nthreads) : _nthreads(nthreads) {}

vector<Vertex> Bicc::getArticulationPoints(Graph& sparseGraph) {
    // BFS - the result of this function should be the BFS spanning tree
    // each vertex should have its parent makred as well as its level
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
    Graph tree;

    // Insert code here

    return tree;
}

vector<Graph> Bicc::findBridges(Graph& graph, Graph& bfsTree) {
    vector<Graph> components;

    // Insert code here

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
