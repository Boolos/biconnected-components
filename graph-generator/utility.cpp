#include "utility.hpp"

std::string csce::utility::duration_string(long long int duration) {
	std::stringstream out;
	out << (duration / 1E9) << " seconds";
	return out.str();
}


csce::graph csce::utility::generate_random(int nvertices, int nedges, bool connected, bool directed) {
	std::random_device rd;
	std::mt19937 mt(rd());
	csce::utility::uniform_distribution<int> dist(0, nvertices - 1);
	
	csce::graph result(nvertices);
	
	const long long int max_edges = directed ? (nvertices * (nvertices - 1)) : (nvertices * (nvertices - 1)) / 2;
	nedges = static_cast<int>(std::min(static_cast<long long int>(nedges), max_edges));
	for(int x=0; x<nedges; ){
		int a = dist(mt);
		int b = dist(mt);
		if(a == b){
			continue; //prevent self loops
		}
		
		csce::vertex va = csce::vertex(a);
		csce::vertex vb = csce::vertex(b);
		if((directed && result.contains(va, vb)) || (!directed && result.contains(va, vb) && result.contains(vb, va))){
			continue; //the edge already exists
		}
		
		result.add(va, vb);
		if(!directed){
			result.add(vb, va);
		}
		
		x++;
	}
	
	if(connected){
		csce::utility::connect_graph(result, directed);
	}
	
	return result;
}

void csce::utility::connect_graph(csce::graph& graph, bool directed){
	auto vertices = graph.vertices();
	auto left_to_visit = graph.vertices();
	for(auto& vertex : vertices){
		std::stack<csce::vertex> s;
		s.push(vertex);
		while(!s.empty()){
			csce::vertex current = s.top();
			s.pop();
			auto neighbors = graph.neighbors(current);
			for(auto& neighbor : neighbors){
				if(left_to_visit.find(neighbor) != left_to_visit.end()){
					s.push(neighbor);
					left_to_visit.erase(neighbor);
				}
			}
		}
		if(!left_to_visit.empty()){
			for(auto& unvisited : left_to_visit){
				if(directed){
					graph.add_undirected(vertex, unvisited);
				} else {
					graph.add(vertex, unvisited);
				}
			}
		}
	}
}

//csce::graph csce::utility::generate_random_bipartite(int nvertices, int nedges, bool directed){
//
//}


csce::graph csce::utility::generate_complete(int nvertices) {
	csce::graph result(nvertices);
	for(int x=0; x<nvertices; x++){
		for(int y=0; y<nvertices; y++){
			if(x != y){
				result.add(csce::vertex(x), csce::vertex(y));
			}
		}
	}
	return result;
}


csce::graph csce::utility::load_from_file(const std::string& file_path) {
	csce::graph g;
	
	std::ifstream file(file_path, std::ios::in);
	if(file.is_open()){
		int n = 0;
		file >> n;
		g.reserve(n);
		
		int x=0, y=0;
		while(file >> x >> y){
			g.add(csce::vertex(x), csce::vertex(y));
		}
		file.close();
	}
	
	return g;
}


void csce::utility::write_to_file(csce::graph& g, const std::string& file_path) {
	std::ofstream file(file_path, std::ios_base::out | std::ios_base::trunc);
	if(file.is_open()){
		file << g.size() << std::endl;
		auto vertices = g.vertices();
		for(auto& vertex : vertices){
			auto& neighbors = g.neighbors(vertex);
			for(auto& neighbor : neighbors){
				file << vertex.str() << " " << neighbor.str() << std::endl;
			}
		}
		file.close();
	}
}