#ifndef UARK_CSCE_BICC
#define UARK_CSCE_BICC

#include <string>
#include <sstream>
#include <queue>
#include <list>
#include <omp.h>
#include "graph.hpp"
#include "vertex.hpp"
#include "edge.hpp"

using namespace std;

namespace csce {
	class Bicc {
	public:
        Bicc(int nthreads);

        vector<Vertex> getArticulationPoints(Graph& sparseGraph);
        Graph breadthFirstSearch(Graph& sparseGraph);
        vector<Graph> findBridges(Graph& graph, Graph& bfsTree);
        vector<Vertex> findArtPointsInParallel(vector<Graph> components, Graph& bfsTree);
        Graph constructAuxGraph(Graph graph);
        bool isArticulationPoint(const Graph& bfs, const Graph& auxGraph) const;

	private:
        int _nthreads;
	};
}

#endif /* UARK_CSCE_BICC */
