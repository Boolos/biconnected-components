#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include "tarjan-vishkin.h"
#include "chaitanya-kothapalli.h"
#include "CK-TV.h"
#include "util.h"
using namespace std;

#define DEBUG

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
	size_t E, u, v;
	fin >> E;
	
	for(size_t i = 0; i < E; i++)
	{
		fin >> u >> v;
		if(g.V() <= u || g.V() <= v)
			g.resize(max(u, v) + 1);
		g.addDirectedEdge(u, v);
	}
	
	fin.close();
	
	vector<BiCC *> algorithms;
	algorithms.push_back(new TarjanVishkin());
	algorithms.push_back(new ChaitanyaKothapalli());
	algorithms.push_back(new CKTV());
	
	for(size_t i = 0; i < algorithms.size(); i++)
	{
		cout << "===== " << algorithms[i]->name() << " =====" << endl;
		
		vector< set<size_t> > bicc;
		clock_t start = clock();
		algorithms[i]->getBiCC(g, bicc);
		
		cout << "Found " << bicc.size() << " biconnected components in " << double(clock() - start) / CLOCKS_PER_SEC << " seconds!" << endl;
		
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
