#include "CK-TV.h"
#include "tarjan-vishkin.h"
#include "util.h"
#include <omp.h>
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
	
	#pragma omp parallel num_threads(this->nthreads)
	{
		vector< set<size_t> > threadBicc;
		
		#pragma omp for
		for(size_t i = 0; i < components.size(); i++)
		{
			if(components[i].size() > 1)
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
						threadBicc.push_back(component);
				}
			}
		}
		
		#pragma omp critical
		{
			bicc.insert(bicc.end(), threadBicc.begin(), threadBicc.end());
		}
	}
	
	for(size_t i = 0; i < bridges.size(); i++)
	{
		bicc.push_back(set<size_t>());
		bicc.back().insert(bridges[i].first);
		bicc.back().insert(bridges[i].second);
	}
}
