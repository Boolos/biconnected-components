#include "utility.hpp"

std::string csce::utility::duration_string(long long int duration) {
	std::stringstream out;
	out << (duration / 1E9) << " seconds";
	return out.str();
}


csce::graph csce::utility::generate_random(int nvertices, int nedges) {
	std::random_device rd;
	std::mt19937 mt(rd());
	csce::utility::uniform_distribution<int> dist(0, nvertices - 1);
	
	csce::graph result(nvertices);
	
	long long int max_edges = (nvertices * (nvertices - 1)) / 2;
	nedges = static_cast<int>(std::min(static_cast<long long int>(nedges), max_edges));
	for(int x=0; x<nedges; ){
		int a = dist(mt);
		int b = dist(mt);
		if(a == b){
			continue; //prevent self loops
		}
		result.add(csce::vertex(a), csce::vertex(b));
		x++;
	}
	
	return result;
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