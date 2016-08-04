#include "chaitanya-kothapalli.h"
#include "util.h"
#include <omp.h>
using namespace std;

// virtual
const char *ChaitanyaKothapalli::name()
{
	return "Chaitanya-Kothapalli";
}

// virtual
void ChaitanyaKothapalli::getBiCC(const Graph &g, vector< set<size_t> > &bicc)
{
	Graph t, nt;
	vector<size_t> parent, level;
	vector< vector<size_t> > components;
	vector<Edge> bridges;
	
	g.spanningTree(&t, &nt, &parent, &level);
	removeBridges(g, t, nt, parent, level, components, &bridges);
	
	#pragma omp parallel num_threads(this->nthreads)
	{
		vector< set<size_t> > localBicc;
		
		#pragma omp for
		for(size_t i = 0; i < components.size(); i++)
		{
			if(components[i].size() > 1)
			{
				Graph aux, tPrime, ntPrime;
				vector<size_t> parentPrime, levelPrime, alias;
				vector< vector<size_t> > componentsPrime;
				
				auxiliaryGraph(g, nt, parent, level, components[i], aux, alias);
				aux.spanningTree(&tPrime, &ntPrime, &parentPrime, &levelPrime);
				removeBridges(aux, tPrime, ntPrime, parentPrime, levelPrime, componentsPrime);
				
				for(size_t j = 0; j < componentsPrime.size(); j++)
				{
					set<size_t> component;
					for(size_t k = 0; k < componentsPrime[j].size(); k++)
						component.insert(alias[componentsPrime[j][k]]);
					if(component.size() > 1)
						localBicc.push_back(component);
				}
			}
		}
		
		#pragma omp critical
		{
			bicc.insert(bicc.end(), localBicc.begin(), localBicc.end());
		}
	}
	
	for(size_t i = 0; i < bridges.size(); i++)
	{
		bicc.push_back(set<size_t>());
		bicc.back().insert(bridges[i].first);
		bicc.back().insert(bridges[i].second);
	}
}

void ChaitanyaKothapalli::removeBridges(const Graph &g, const Graph &t, const Graph &nt, vector<size_t> &parent, vector<size_t> &level, vector< vector<size_t> > &components, vector<Edge> *bridges)
{
	Graph gPrime(g);
	
	vector<bool> marked(t.V(), false);
	size_t u, v, lu, lv;
	
	for(size_t i = 0; i < nt.E(); i++)
	{
		u = nt.edges()[i].first, v = nt.edges()[i].second;
		while(u != v)
		{
			lu = level[u];
			lv = level[v];
			if(lu >= lv)
			{
				marked[u] = true;
				u = parent[u];
			}
			if(lv >= lu)
			{
				marked[v] = true;
				v = parent[v];
			}
		}
	}
	
	for(size_t i = 1; i < g.V(); i++)
	{
		if(!marked[i])
		{
			gPrime.removeEdgeSafe(i, parent[i]);
			if(bridges)
				bridges->push_back(Edge(i, parent[i]));
		}
	}
	
	gPrime.spanningTree(NULL, NULL, NULL, NULL, &components);
}

void ChaitanyaKothapalli::auxiliaryGraph(const Graph &g, const Graph &nt, const vector<size_t> &parent, const vector<size_t> &level, const vector<size_t> &component, Graph &aux, vector<size_t> &alias)
{
	vector<size_t> antiAlias;
	aux.copyComponent(g, component, antiAlias, alias);
	for(size_t i = 0; i < component.size(); i++)
	{
		size_t u = component[i];
		for(list<size_t>::const_iterator j = nt.adj(u).begin(); j != nt.adj(u).end(); ++j)
		{
			size_t v = *j, a, b;
			if(u < v)
			{
				size_t x = LCA(parent, level, u, v, &a, &b), xPrime = aux.V();
				alias.push_back(x);
				x = antiAlias[x], a = antiAlias[a], b = antiAlias[b];
				aux.removeEdgeSafe(x, a);
				aux.removeEdgeSafe(x, b);
				aux.addVertex();
				aux.addEdge(xPrime, a);
				aux.addEdge(xPrime, b);
			}
		}
	}
}
