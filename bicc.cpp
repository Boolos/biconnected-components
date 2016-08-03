#include "bicc.hpp"

using namespace std;
using namespace csce;

Bicc::Bicc() {}

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
    auto artPoints = findArtPointsInParallel(components);

    return artPoints;
}

Graph Bicc::breadthFirstSearch(Graph& sparseGraph) {
    Graph tree;

    return tree;
}

vector<Graph> Bicc::findBridges(Graph& graph, Graph& bfsTree) {
    vector<Graph> components;

    return components;
}

vector<Vertex> Bicc::findArtPointsInParallel(vector<Graph> components) {
    vector<Vertex> artPoints;

    return artPoints;
}


