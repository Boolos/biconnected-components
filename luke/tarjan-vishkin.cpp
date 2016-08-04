#include <vector>
#include <list>
#include <map>
#include <set>
#include <tuple>
#include <algorithm>
#include <queue>
#include <omp.h>
#include "tarjan-vishkin.h"
#include "util.h"
using namespace std;

// virtual
const char *TarjanVishkin::name()
{
	return "Tarjan-Vishkin";
}

// virtual
void TarjanVishkin::getBiCC(const Graph &g, vector< set<size_t> > &bicc)
{
	g.spanningTree(&t, &nt, &parent, &level);
	eulerTour();
	preorderVertices();
	findLow();
	auxiliaryGraph(g);
	gPrime.spanningTree(NULL, NULL, NULL, NULL, &components);
	remapAuxiliaryGraph(bicc);
	
	for(size_t i = 0; i < bicc.size(); i++)
	{
		if(bicc[i].size() == 0)
		{
			bicc.erase(bicc.begin() + i);
			i--;
		}
	}
}

void TarjanVishkin::eulerTour()
{
	const vector<Edge> &edges = t.edges();
	vector<size_t> first(t.V(), -1), next(edges.size(), -1), twin(edges.size(), -1);
	
	for(size_t i = 0; i < edges.size(); i++) // parallelizable
	{
		twin[distance(edges.begin(), lower_bound(edges.begin(), edges.end(), reverseEdge(edges[i])))] = i;
		if(i == 0 || edges[i].first != edges[i-1].first)
			first[edges[i].first] = i;
		else if(edges[i].first == edges[i-1].first)
			next[i-1] = i;
	}
	
	succ.resize(edges.size());
	
	#pragma omp parallel for num_threads(this->nthreads)
	for(size_t i = 0; i < edges.size(); i++)
	{
		if(next[twin[i]] != -1)
			succ[i] = next[twin[i]];
		else
			succ[i] = first[edges[i].second];
	}
}

void TarjanVishkin::preorderVertices()
{
	preorder.resize(t.V(), -1);
	preorder[t.edges()[0].first] = 0;
	preorder[t.edges()[0].second] = 1;
	
	size_t i = 0, j = 2;
	do
	{
		size_t k = t.edges()[succ[i]].second;
		if(preorder[k] == -1)
			preorder[t.edges()[succ[i]].second] = j++;
		i = succ[i];
	}
	while(i != 0);
}

void TarjanVishkin::findLow()
{
	low.resize(nt.V());
	
	#pragma omp parallel for num_threads(this->nthreads)
	for(size_t i = 0; i < nt.V(); i++) // parallelize
		low[i] = i;
	
	bool changed = true;
	while(changed)
	{
		changed = false;
		
		#pragma omp parallel for num_threads(this->nthreads)
		for(size_t i = 0; i < nt.V(); i++)
		{
			for(list<size_t>::const_iterator j = t.adj(i).begin(); j != t.adj(i).end(); ++j)
			{
				if(level[i] < level[*j] && level[low[*j]] < level[low[i]])
				{
					low[i] = low[*j];
					changed = true;
				}
			}
			for(list<size_t>::const_iterator j = nt.adj(i).begin(); j != nt.adj(i).end(); ++j)
			{
				if(level[low[*j]] < level[low[i]])
				{
					low[i] = low[*j];
					changed = true;
				}
			}
		}
	}
}

void TarjanVishkin::auxiliaryGraph(const Graph &g)
{
	gPrime.resize(g.V() + nt.E());
	vector<size_t> N(g.E(), 0);
	
	size_t ti = 0, nti = 0;
	for(size_t i = 0; i < g.E(); i++) // to parallelize, need a vector<bool> isTreeEdge
	{
		if(nti >= nt.E() || g.edges()[i] == t.edges()[ti])
			ti++;
		else
		{
			N[i] = 1;
			nti++;
		}
	}
	
	prefixSum(N);
	
	ti = 0, nti = 0;
	for(size_t i = 0; i < g.E(); i++) // to parallelize, need a vector<bool> isTreeEdge
	{
		size_t u = g.edges()[i].first, v = g.edges()[i].second;
		if(nti >= nt.edges().size() || g.edges()[i] == t.edges()[ti])
		{
			if(level[u] < level[v] && level[low[v]] <= level[u])
				gPrime.addEdge(u, v);
			ti++;
		}
		else
		{
			size_t p = LCA(parent, level, u, v);
			if(preorder[v] < preorder[u])
				gPrime.addEdge(u, N[i] + g.V() - 1);
			if(u < v && p != u && p != v)
				gPrime.addEdge(u, v);
			nti++;
		}
	}
}

void TarjanVishkin::remapAuxiliaryGraph(vector< set<size_t> > &bicc)
{
	bicc.resize(components.size() - 1);
	for(size_t i = 1; i < components.size(); i++)
	{
		for(size_t j = 0; j < components[i].size(); j++)
		{
			// only consider tree edges (since non-tree edges include already-included vertices)
			if(components[i][j] < t.V())
			{
				bicc[i-1].insert(components[i][j]);
				bicc[i-1].insert(parent[components[i][j]]);
			}
		}
	}
}
