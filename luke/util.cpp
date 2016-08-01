#include "util.h"
using namespace std;

ostream &operator<<(ostream &out, const Graph &g)
{
	for(size_t i = 0; i < g.V(); i++)
	{
		cout << char('a' + i) << ": ";
		for(list<size_t>::const_iterator j = g.adj(i).begin(); j != g.adj(i).end(); ++j)
			cout << char('a' + *j) << " ";
		cout << endl;
	}
	return out;
}

Edge reverseEdge(const Edge &e)
{
	return Edge(e.second, e.first);
}

size_t LCA(const vector<size_t> &parent, const vector<size_t> &level, size_t u, size_t v, size_t *a, size_t *b)
{
	size_t lu, lv;
	while(u != v)
	{
		if(a) *a = u;
		if(b) *b = v;
		lu = level[u];
		lv = level[v];
		if(lu >= lv)
			u = parent[u];
		if(lv >= lu)
			v = parent[v];
	}
	return u;
}

void prefixSum(vector<size_t> &v)
{
	size_t sum = v[0];
	for(size_t i = 1; i < v.size(); i++) // parallelize?
	{
		sum += v[i];
		v[i] = sum;
	}
}

void biccToArticulationPoints(const Graph &g, const vector< set<size_t> > &bicc, set<size_t> &articulationPoints)
{
	vector<size_t> count(g.V(), 0);
	for(size_t i = 0; i < g.V(); i++) // parallelize
	{
		for(size_t j = 0; j < bicc.size() && count[i] < 2; j++)
		{
			count[i] += bicc[j].count(i);
			if(count[i] == 2)
				articulationPoints.insert(i);
		}
	}
}
