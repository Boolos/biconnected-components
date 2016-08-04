#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <chrono>
#include <climits>
#include "tarjan-vishkin.h"
#include "chaitanya-kothapalli.h"
#include "CK-TV.h"
#include "util.h"
using namespace std;

// Loads a graph from a file
// The first line of the file is the number of vertices
// Remaining lines are the edges [fromVertex toVertex]
void loadGraph(const char *filename, Graph &g)
{
	ifstream fin;
	fin.open(filename);
	
	size_t V, u, v;
	fin >> V;
	g.resize(V);
	
	while(!fin.fail())
	{
		fin >> u >> v;
		if(!fin.fail())
			g.addEdgeSafe(u, v);
	}
	
	fin.close();
}

// Connects a (potentially) unconnected graph
void connectGraph(Graph &g)
{
	vector< vector<size_t> > components;
	g.spanningTree(NULL, NULL,  NULL, NULL, &components);
	for(size_t i = 1; i < components.size(); i++)
		g.addEdge(components[i-1][0], components[i][0]);
}

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		cout << "Usage: bicc [graph.txt] [nthreads]" << endl;
		return 0;
	}
	
	size_t nthreads = 1;
	if(argc > 2)
		nthreads = atoi(argv[2]);
	
	Graph g;
	loadGraph(argv[1], g);
	connectGraph(g);
	
	double avgDegree = double(g.E() / 2) / g.V();
	size_t minDegree = INT_MAX;
	size_t maxDegree = 0;
	for(size_t i = 0; i < g.V(); i++)
	{
		if(g.adj(i).size() < minDegree)
			minDegree = g.adj(i).size();
		if(g.adj(i).size() > maxDegree)
			maxDegree = g.adj(i).size();
	}
	
	cout << "Run stats ====================================\n"
	     << "File:           " << argv[1] << "\n"
	     << "Minimum degree: " << minDegree << "\n"
	     << "Maximum degree: " << maxDegree << "\n"
	     << "Average degree: " << avgDegree << "\n"
	     << "Thread count:   " << nthreads << endl;
	
#ifdef DEBUG
	cout << "Graph:\n" << g << endl;
#endif
	
	vector<BiCC *> algorithms;
	algorithms.push_back(new TarjanVishkin(nthreads));
	algorithms.push_back(new ChaitanyaKothapalli(nthreads));
	algorithms.push_back(new CKTV(nthreads));
	
	for(size_t i = 0; i < algorithms.size(); i++)
	{
		cout << algorithms[i]->name() << endl;
		
		vector< set<size_t> > bicc;
		
		chrono::high_resolution_clock::time_point start_time = chrono::high_resolution_clock::now();
		algorithms[i]->getBiCC(g, bicc);
		chrono::high_resolution_clock::time_point stop_time = chrono::high_resolution_clock::now();
		size_t nanoseconds = chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
		
		cout << nanoseconds / 1e9 << " seconds, " << bicc.size() << " biconnected components" << endl;
		
#ifdef DEBUG
		for(size_t j = 0; j < bicc.size(); j++)
		{
			cout << j << ": ";
			for(set<size_t>::iterator k = bicc[j].begin(); k != bicc[j].end(); ++k)
				cout << char('a' + *k) << " ";
			cout << endl;
		}
		
		set<size_t> articulationPoints;
		biccToArticulationPoints(g, bicc, articulationPoints);
		
		cout << "Articulation points: ";
		for(set<size_t>::iterator j = articulationPoints.begin(); j != articulationPoints.end(); ++j)
			cout << char('a' + *j) << " ";
		cout << endl;
#endif
		
		delete algorithms[i];
	}
	
	return 0;
}
