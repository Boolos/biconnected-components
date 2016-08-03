#ifndef UARK_CSCE_BICC
#define UARK_CSCE_BICC

#include <string>
#include <sstream>
#include "graph.hpp"
#include "vertex.hpp"

using namespace std;

namespace csce {
	class Bicc {
	public:
        Bicc();

        vector<Vertex> getArticulationPoints(Graph& sparseGraph);
        Graph breadthFirstSearch(Graph& sparseGraph);
        vector<Graph> findBridges(Graph& graph, Graph& bfsTree);
        vector<Vertex> findArtPointsInParallel(vector<Graph> components);

	private:
	};
}

#endif /* UARK_CSCE_BICC */
