#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include "tarjan-vishkin.h"
#include "chaitanya-kothapalli.h"
#include "util.h"
using namespace std;

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		cout << "Usage: bicc [graph.txt]" << endl;
		return 0;
	}
	
	ifstream fin;
	fin.open(argv[1]);
	
	Graph g;
	size_t V, E;
	fin >> V;
	g.resize(V);
	
	for(size_t u = 0; u < V; u++)
	{
		fin >> E;
		for(size_t i = 0; i < E; i++)
		{
			size_t v;
			fin >> v;
			g.addEdge(u, v);
		}
	}
	
	fin.close();
	
	vector<BiCC *> algorithms;
	algorithms.push_back(new TarjanVishkin());
	algorithms.push_back(new ChaitanyaKothapalli());
	
	for(size_t i = 0; i < algorithms.size(); i++)
	{
		cout << "===== " << algorithms[i]->name() << " =====" << endl;
		
		vector< set<size_t> > bicc;
		algorithms[i]->getBiCC(g, bicc);
		
		cout << "Found " << bicc.size() << " biconnected components!" << endl;
		
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
		
		delete algorithms[i];
	}
	
	return 0;
}
