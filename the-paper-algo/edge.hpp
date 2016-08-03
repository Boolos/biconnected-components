#ifndef UARK_CSCE_EDGE_HEADER
#define UARK_CSCE_EDGE_HEADER

#include "vertex.hpp"
#include <string>
#include <sstream>

using namespace std;

namespace csce {
	class Edge {
	public:
        Edge(size_t uId, size_t vId);
        Edge(Vertex u, Vertex v);

        bool isBridge;

        Vertex getLca() const;
        Vertex getU() const;
        Vertex getV() const;
        Vertex setLca(size_t lca);
        Vertex setLca(const Vertex lca);
        string str() const;

		bool operator ==(const Edge& other) const {
            return this->str() == other.str();
		}
	private:
        Vertex _u;
        Vertex _v;
        Vertex _lca;
	};
}

#endif /* UARK_CSCE_EDGE_HEADER */
