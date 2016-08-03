#include "graph.hpp"

csce::graph::graph() {}

csce::graph::graph(int n) {
	this->reserve(n);
}

void csce::graph::reserve(int n) {
	this->_graph.reserve(n);
	for(int x=0; x<n; x++){
		this->add(csce::vertex(x));
	}
}

std::size_t csce::graph::size() const {
	return this->_graph.size();
}

std::unordered_set<csce::vertex> csce::graph::vertices() const {
	std::unordered_set<csce::vertex> result;
	for(auto& vertex : this->_graph){
		result.insert(vertex.first);
	}
	return result;
}

std::size_t csce::graph::degree(const csce::vertex& v) const {
	auto result = this->_graph.find(v);
	if(result != this->_graph.end()){
		return result->second.size();
	}
	return 0; //the graph does not contain the vertex, so the degree is zero
}

std::unordered_set<csce::vertex>& csce::graph::neighbors(const csce::vertex& v) {
	return this->_graph.at(v);
}

void csce::graph::add(const csce::vertex& a, const csce::vertex& b) {
	this->add(a);
	this->add(b);
	this->_graph[a].insert(b);
}

void csce::graph::add_undirected(const csce::vertex& a, const csce::vertex& b) {
	this->add(a);
	this->add(b);
	this->_graph[a].insert(b);
	this->_graph[b].insert(a);
}

void csce::graph::add(const csce::vertex& v) {
	if(!this->contains(v)){
		this->_graph[v] = std::unordered_set<csce::vertex>();
	}
}

bool csce::graph::contains(const csce::vertex& v) const {
	return this->_graph.find(v) != this->_graph.end();
}

bool csce::graph::contains(const csce::vertex& a, const csce::vertex& b) const {
	if(!this->contains(a) || !this->contains(b)){
		return false;
	}
	
	return this->_graph.at(a).find(b) != this->_graph.at(a).end();
}