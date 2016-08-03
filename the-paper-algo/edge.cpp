#include "edge.hpp"

using namespace csce;
using namespace std;

Edge::Edge(size_t uId, size_t vId) : _u(Vertex(uId)), _v(Vertex(vId)), _lca(Vertex(-1)) { }

Edge::Edge(Vertex u, Vertex v) : _u(u), _v(v), _lca(Vertex(-1)) { }

Vertex Edge::getLca() const {
    return this->_lca;
}

Vertex Edge::getU() const {
    return this->_u;
}

Vertex Edge::getV() const {
    return this->_v;
}

Vertex Edge::setLca(size_t lca) {
    this->setLca(Vertex(lca));
}

Vertex Edge::setLca(const Vertex lca) {
    this->_lca = lca;
}
        
string Edge::str() const {
    auto first = _u.getId() > _v.getId() ? _u.getId() : _v.getId();
    auto second = first == _u.getId() ? _v.getId() : _u.getId();
    ostringstream oss;
    oss << first << "-" << second;
    return oss.str();
}
