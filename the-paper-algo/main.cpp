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
    csce::Graph testGraph;
    testGraph.add(0,1).add(0,5).add(0,6)
        .add(1,2).add(1,3).add(1,4).add(1,0)
        .add(2,3).add(2,1)
        .add(3,2).add(3,1)
        .add(4,1).add(4,5)
        .add(5,4).add(5,0)
        .add(6,0).add(6,8).add(6,7)
        .add(7,6).add(7,8)
        .add(8,6).add(8,7);

    vector<csce::Graph> biconnectedComponents;
    
    csce::Graph comp1;
    comp1.add(1,2).add(2,3).add(3,1);
    biconnectedComponents.push_back(comp1);
    
    csce::Graph comp2;
    comp2.add(1,4).add(4,5).add(5,0).add(0,1);
    biconnectedComponents.push_back(comp2);

    csce::Graph comp3;
    comp3.add(0,6);
    biconnectedComponents.push_back(comp3);

    csce::Graph comp4;
    comp4.add(6,8).add(8,7).add(7,6);
    biconnectedComponents.push_back(comp4);

    vector<csce::Vertex> testArtPoints;
    testArtPoints.push_back(0);
    testArtPoints.push_back(1);
    testArtPoints.push_back(6);
	
	long long int duration = 0;
	
	int nthreads = std::thread::hardware_concurrency();
	csce::Bicc bicc(nthreads);
	
	std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
	//run the algorithm on the input, timing
	vector<csce::Vertex> outputArtPoints = bicc.getArticulationPoints(testGraph);
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "Computing articulation vertices ... " << std::flush;
	std::chrono::high_resolution_clock::time_point stop_time = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
	
	std::cout << "done in " << duration_string(duration) << std::endl;
	
	std::cout << "There were " << outputArtPoints.size() << " articulation vertices. " << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	
	for(csce::Vertex& v : outputArtPoints){
		std::cout << v.str() << std::endl;
	}
	
	//check correctness
	if(outputArtPoints == testArtPoints){
		std::cout << "Correct" << std::endl;
		}
		
		else
			std::cout << "Incorrect" << std::endl;
	
	return 0;
}
