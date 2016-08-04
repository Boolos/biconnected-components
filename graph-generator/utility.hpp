#ifndef utility_hpp
#define utility_hpp

#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <cmath>
#include <unordered_set>
#include <thread>
#include <stack>

#include "graph.hpp"

namespace csce {
	namespace utility {
		template<typename f, typename... args>
		long long int duration_function(f func, args&&... arg){
			std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
			func(std::forward<args>(arg)...);
			std::chrono::high_resolution_clock::time_point stop_time = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
		}
		
		
		std::string duration_string(long long int duration);
		
		
		template<typename T>
		using uniform_distribution = typename std::conditional<
			std::is_integral<T>::value, std::uniform_int_distribution<T>,
			typename std::conditional<std::is_floating_point<T>::value, std::uniform_real_distribution<T>, void>::type
		>::type;
		
		
		csce::graph generate_random(int nvertices, int nedges, bool connected, bool directed);
		csce::graph generate_complete(int nvertices);
		void connect_graph(csce::graph& graph, bool directed);
		csce::graph generate_random_bipartite(int nvertices, int nedges, bool connected, bool directed);
		csce::graph generate_complete_bipartite(int nvertices);
		csce::graph load_from_file(const std::string& file_path);
		void write_to_file(csce::graph& g, const std::string& file_path);
		
	}
}

#endif /* utility_hpp */
