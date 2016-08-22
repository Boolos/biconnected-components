#include <iostream>
#include <fstream>
#include <unistd.h>
#include <chrono>
#include <vector>
#include <string>
#include <thread>
#include <unordered_set>
#include <set>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <utility>
#include <unistd.h>

#include "graph.hpp"
#include "bicc.hpp"

using namespace std;

string duration_string(long long int duration) {
		std::stringstream out;
		out << (duration / 1E9) << " seconds";
		return out.str();
	}
	
//read text file and construct graph with adjacency list
csce::Graph load_from_file(const std::string& file_path) {
	csce::Graph G;
	
		std::ifstream file(file_path, std::ios::in);
		if(file.is_open()){
			int n = 0;
			file >> n;
		
		int x=0, y=0;
			while(file >> x >> y){
				G.add(csce::Vertex(x), csce::Vertex(y));
		}
		file.close();
	}
	
	return G;
}
	
int main(int argc, char **argv)
{	
	long long int duration = 0;
	
	int nthreads = 1; //the default number of threads. This can be changed by the -t runtime argument.
	std::string input_file_path; //where to load data from, if anywhere. If this is not specified, the data will be generated at runtime.
	csce::Bicc bicc(nthreads);
	
	//using input graph
	
	int c;
	while((c = getopt(argc, argv, "f:t:")) != -1){
		switch(c){
			case 'f':
				if(optarg != NULL){
					input_file_path = std::string(optarg);
				}
				break;
				
			case 't':
				if(optarg != NULL){
					nthreads = std::stoi(optarg);
				}
				break;
		}
	}
	
	std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
	
	csce::Graph inputGraph = load_from_file (input_file_path);
	vector<csce::Vertex> outputArtPoints = bicc.getArticulationPoints(inputGraph);
	std::cout << "Computing articulation vertices ... " << std::flush;
	std::chrono::high_resolution_clock::time_point stop_time = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
	
	std::cout << "done in " << duration_string(duration) << std::endl;
	csce::Graph bfsTree = bicc.breadthFirstSearch(inputGraph);
	vector<csce::Graph> components = bicc.findBridges(inputGraph, bfsTree);
	csce::Graph G_T = inputGraph.difference(bfsTree);
	
	return 0;
}
