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
	std::vector<csce::vertex> components;
	for(auto& vertex : vertices){
		if(left_to_visit.find(vertex) != left_to_visit.end()){
			components.push_back(vertex);
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
		}
	}
	
	for(std::size_t x=0; x<components.size() - 1; x++){
		if(directed){
			graph.add(components[x], components[x+1]);
		} else {
			graph.add_undirected(components[x], components[x+1]);
		}
	}
}

csce::graph csce::utility::generate_random_bipartite(int nvertices, int nedges, bool connected, bool directed){
	csce::graph bipartite(nvertices);
	std::vector<csce::vertex> va;
	std::vector<csce::vertex> vb;
	auto vertices = bipartite.vertices();
	int x = 0;
	
	std::random_device rd;
	std::mt19937 mt(rd());
	
	for(auto& vertex : vertices){
		if(x % 2 == 0){
			va.push_back(vertex);
		} else {
			vb.push_back(vertex);
		}
		x++;
	}
	
	csce::utility::uniform_distribution<std::size_t> dist_a(0UL, va.size() - 1UL);
	csce::utility::uniform_distribution<std::size_t> dist_b(0UL, vb.size() - 1UL);
	
	for(int x=0; x<nedges; x++){
		std::size_t a = dist_a(mt);
		std::size_t b = dist_b(mt);
		
		if((directed && bipartite.contains(va[a], vb[b])) || (!directed && bipartite.contains(va[a], vb[b]) && bipartite.contains(vb[b], va[a]))){
			continue; //the edge already exists
		}
		
		bipartite.add(va[a], vb[b]);
		if(!directed){
			bipartite.add(vb[b], va[a]);
		}
	}
	
	if(connected && !va.empty() && !vb.empty()){
		for(auto& vertex : va){
			if(bipartite.degree(vertex) == 0){
				bipartite.add(vertex, vb[0]);
				if(!directed){
					bipartite.add(vb[0], vertex);
				}
			}
		}
		
		for(auto& vertex : vb){
			if(bipartite.degree(vertex) == 0){
				bipartite.add(vertex, va[0]);
				if(!directed){
					bipartite.add(va[0], vertex);
				}
			}
		}
	}
	
	return bipartite;
}

csce::graph csce::utility::generate_complete_bipartite(int nvertices) {
	csce::graph bipartite(nvertices);
	std::vector<csce::vertex> va;
	std::vector<csce::vertex> vb;
	auto vertices = bipartite.vertices();
	int vertex_count = 0;
	
	for(auto& vertex : vertices){
		if(vertex_count % 2 == 0){
			va.push_back(vertex);
		} else {
			vb.push_back(vertex);
		}
		vertex_count++;
	}
	
	for(std::size_t x=0; x<va.size(); x++){
		for(std::size_t y=0; y<vb.size(); y++){
			bipartite.add_undirected(va[x], vb[y]);
		}
	}
	
	return bipartite;
}


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