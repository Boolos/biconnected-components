#ifndef graph_hpp
#define graph_hpp

#include <unordered_map>
#include <unordered_set>

#include "vertex.hpp"

namespace csce {
	class graph {
	public:
		
		graph();
		
		graph(int n);
		
		void reserve(int n);
		
		std::size_t size() const;
		
		std::unordered_set<csce::vertex> vertices() const;
		
		/**
		 * Returns the number of vertices that are incident to the specified
		 * vertex. If no such vertex exists, 0 is returned.
		 */
		std::size_t degree(const csce::vertex& v) const;
		
		/**
		 * Returns a reference to the collection of neighboring vertices
		 * to vertex v. If no such vertex exists in the graph, an out_of_range
		 * exception is thrown.
		 */
		std::unordered_set<csce::vertex>& neighbors(const csce::vertex& v);
		
		void add(const csce::vertex& a, const csce::vertex& b);
		
		void add_undirected(const csce::vertex& a, const csce::vertex& b);
		
		void add(const csce::vertex& v);
		
		/**
		 * Returns true if the graph contains the specified vertex
		 * or false if the graph does not contain the specified vertex.
		 */
		bool contains(const csce::vertex& v) const;
		
		/**
		 * Returns true if the graph contains an edge from vertex a to vertex b
		 * or false if no such edge exists.
		 */
		bool contains(const csce::vertex& a, const csce::vertex& b) const;
		
	private:
		std::unordered_map<csce::vertex, std::unordered_set<csce::vertex>> _graph;
	};
}

#endif /* graph_hpp */