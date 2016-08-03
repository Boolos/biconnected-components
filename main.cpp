#include <iostream>
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

#include "utility.hpp"
#include "test.hpp"
#include "graph.hpp"
#include "connected_components.hpp"

int main(int argc, char* argv[]) {
	int n = 320; //the default number of vertices. This can be changed by specifying the -n runtime argument.
	int nedges = 0;
	int thread_count = std::thread::hardware_concurrency(); //the default number of threads is the number of hardware cores on the current computer. This can be changed by the -t runtime argument.
	std::string input_file_path; //where to load data from, if anywhere. If this is not specified, the data will be generated at runtime.
	std::string output_file_path; //where to write out the generated data, if anywhere.
	long long int duration = 0;
	bool debug = false;
	bool test_mode = false;
	
	csce::graph graph;
	
	int c;
	while((c = getopt(argc, argv, ":dDf:m:n:o:t:")) != -1){
		switch(c){
			case 'd':
				debug = true;
				break;
				
			case 'D':
				test_mode = true;
				break;
				
			case 'f':
				if(optarg != NULL){
					input_file_path = std::string(optarg);
				}
				break;
				
			case 'm':
				if(optarg != NULL){
					std::stringstream argument_stream(optarg);
					argument_stream >> nedges;
				}
				break;
				
			case 'n':
				if(optarg != NULL){
					std::stringstream argument_stream(optarg);
					argument_stream >> n;
				}
				break;
				
			case 'o':
				if(optarg != NULL){
					output_file_path = std::string(optarg);
				}
				break;
				
			case 't':
				if(optarg != NULL){
					std::stringstream argument_stream(optarg);
					argument_stream >> thread_count;
				}
				break;
				
			case '?':
				break;
		}
	}
	
	if(nedges == 0){
		nedges = 2 * n;
	}
	
	//
	// run unit tests
	//
	if(test_mode){
		csce::test(debug).run();
	}
	
	
	//
	//first - load the values into the array, either by populating it
	//        with random values or by reading the values from a file.
	//
	if(input_file_path.empty()){
		//no input file was specified, so populate the array with random numbers
		std::cout << "Populating graph with " << n << " vertices and " << nedges << " edges ... " << std::flush;
		graph = csce::utility::generate_random(n, nedges);
	} else {
		//load from the specified file
		std::cout << "Populating graph with data from the file (" << input_file_path << ") ... " << std::flush;
		graph = csce::utility::load_from_file(input_file_path);
	}
	std::cout << "done." << std::endl;
	
	
	//
	//second - if the output file path was specified, write the array to the output file
	//         so it can be used in subsequent runs of this process.
	//
	if(!output_file_path.empty()){
		std::cout << "Writing graph to file (" << output_file_path << ") ... " << std::flush;
		csce::utility::write_to_file(graph, output_file_path);
		std::cout << "done." << std::endl;
	}
	
	csce::connected_components algorithm(thread_count);
	
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "Computing articulation vertices ... " << std::flush;
	
	std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
	std::vector<csce::vertex> result = algorithm.find_articulation_vertices(graph);
	std::chrono::high_resolution_clock::time_point stop_time = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
	
	std::cout << "done in " << csce::utility::duration_string(duration) << std::endl;
	
	std::cout << "There were " << result.size() << " articulation vertices. " << std::flush;
	std::cout << "-------------------------------------------" << std::endl;
	for(csce::vertex& v : result){
		std::cout << v << std::endl;
	}
	std::cout << "-------------------------------------------" << std::endl;
	
	return 0;
}
