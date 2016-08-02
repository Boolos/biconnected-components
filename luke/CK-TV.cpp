#include "CK-TV.h"
#include "tarjan-vishkin.h"
#include "util.h"
using namespace std;

// virtual
const char *CKTV::name()
{
	return "Chaitanya-Kothapalli (Tarjan-Vishkin on 2CC)";
}

// virtual
void CKTV::getBiCC(const Graph &g, vector< set<size_t> > &bicc)
{
	Graph t, nt;
	vector<size_t> parent, level;
	vector< vector<size_t> > components;
	vector<Edge> bridges;
	
	g.spanningTree(&t, &nt, &parent, &level);
	removeBridges(g, t, nt, parent, level, components, &bridges);
	
	for(size_t i = 0; i < components.size(); i++) // parallelize
	{
		TarjanVishkin tv;
		Graph sub;
		vector< set<size_t> > localBicc;
		vector<size_t> antiAlias, alias;
		
		sub.copyComponent(g, components[i], antiAlias, alias);
		tv.getBiCC(sub, localBicc);
		
		for(size_t j = 0; j < localBicc.size(); j++)
		{
			set<size_t> component;
			for(set<size_t>::iterator k = localBicc[j].begin(); k != localBicc[j].end(); ++k)
				component.insert(alias[*k]);
			if(component.size() > 1)
				bicc.push_back(component);
		}
	}
	
	for(size_t i = 0; i < bridges.size(); i++)
	{
		bicc.push_back(set<size_t>());
		bicc.back().insert(bridges[i].first);
		bicc.back().insert(bridges[i].second);
	}
}
