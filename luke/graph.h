#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <list>

typedef std::pair<size_t, size_t> Edge;

class Graph
{
public:
	void resize(size_t v);
	void addVertex();
	void addEdge(size_t u, size_t v);
	void addDirectedEdge(size_t u, size_t v);
	void addEdgeSafe(size_t u, size_t v);
	void addDirectedEdgeSafe(size_t u, size_t v);
	void removeEdge(size_t u, size_t v);
	void removeDirectedEdge(size_t u, size_t v);
	void removeEdgeSafe(size_t u, size_t v);
	void removeDirectedEdgeSafe(size_t u, size_t v);
	std::list<size_t> &adj(size_t u);
	const std::list<size_t> &adj(size_t u) const;
	const std::vector<Edge> &edges() const;
	size_t V() const;
	size_t E() const;
	
	void copyComponent(const Graph &g, const std::vector<size_t> &component, std::vector<size_t> &antiAlias, std::vector<size_t> &alias);
	void spanningTree(Graph *t = NULL, Graph *nt = NULL, std::vector<size_t> *parent = NULL, std::vector<size_t> *level = NULL, std::vector< std::vector<size_t> > *components = NULL) const;
private:
	std::vector< std::list<size_t> > m_adj;
	std::vector<Edge> m_edges;
};

#endif
