#include "graph.h"
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <tuple>
using std::cout;
using std::endl;
using std::ostream;
using std::set;
using std::vector;
using std::pair;
using std::make_pair;
using std::map;

size_t leastCommonAncestor(const Graph &t, size_t u, size_t v, size_t *c1 = NULL, size_t *c2 = NULL)
{
	if(!t.isTree())
		return leastCommonAncestor(Graph::BFSTree(t), u, v, c1, c2);
	size_t lu, lv;
	while(u != v)
	{
		if(c1) *c1 = u;
		if(c2) *c2 = v;
		lu = t.level(u);
		lv = t.level(v);
		if(lu >= lv)
			u = t.parent(u);
		if(lv >= lu)
			v = t.parent(v);
	}
	return u;
}

vector<Graph> removeBridges(const Graph &g, const Graph &t, const set< pair<size_t, size_t> > &nonTreeEdges)
{
	Graph gPrime(g);
	
	set< pair<size_t, size_t> > bridges;
	for(size_t i = 0; i < g.V(); i++)
		for(set<size_t>::const_iterator j = t.adj(i).begin(); j != t.adj(i).end(); ++j)
			if(i < *j)
				bridges.insert(make_pair(i, *j));
	
	for(set< pair<size_t, size_t> >::const_iterator i = nonTreeEdges.begin(); i != nonTreeEdges.end(); ++i)
	{
		size_t u = i->first, v = i->second, lu, lv;
		while(u != v)
		{
			lu = t.level(u);
			lv = t.level(v);
			if(lu >= lv)
			{
				if(u < t.parent(u))
					bridges.erase(make_pair(u, t.parent(u)));
				else
					bridges.erase(make_pair(t.parent(u), u));
				u = t.parent(u);
			}
			if(lv >= lu)
			{
				if(v < t.parent(v))
					bridges.erase(make_pair(v, t.parent(v)));
				else
					bridges.erase(make_pair(t.parent(v), v));
				v = t.parent(v);
			}
		}
	}
	
	for(set< pair<size_t, size_t> >::iterator i = bridges.begin(); i != bridges.end(); ++i)
		gPrime.removeEdge(i->first, i->second);
	
	vector< vector<size_t> > components;
	Graph::BFSTree(gPrime, &components);
	
	vector<Graph> forest(components.size());
	for(size_t i = 0; i < components.size(); i++)
		forest[i] = Graph(gPrime, components[i]);
	
	return forest;
}

Graph auxiliaryGraph(const Graph &g)
{
	set< pair<size_t, size_t> > nonTreeEdges;
	
	Graph gPrime(g);
	Graph t = Graph::BFSTreeRooted(g, 0, &nonTreeEdges);
	
	for(set< pair<size_t, size_t> >::iterator j = nonTreeEdges.begin(); j != nonTreeEdges.end(); ++j)
	{
		size_t b1, b2;
		size_t x = leastCommonAncestor(t, j->first, j->second, &b1, &b2);
		size_t xPrime = gPrime.V();
		gPrime.removeEdge(x, b1);
		gPrime.removeEdge(x, b2);
		gPrime.addVertex(g.vertex(x));
		gPrime.addEdge(x, xPrime);
		gPrime.addEdge(xPrime, b1);
		gPrime.addEdge(xPrime, b2);
	}
	
	return gPrime;
}

void printGraph(const Graph &g, ostream &out = cout)
{
	for(size_t i = 0; i < g.V(); i++)
	{
		cout << char('a' + g.vertex(i)) << ": ";
		for(set<size_t>::const_iterator j = g.adj(i).begin(); j != g.adj(i).end(); ++j)
			out << char('a' + g.vertex(*j)) << " ";
		out << endl;
	}
}

ostream &operator<<(ostream &out, const Graph &g)
{
	printGraph(g, out);
	return out;
}

set< set<size_t> > biconnectedComponents(const Graph &g)
{
	set< set<size_t> > components;
	set< pair<size_t, size_t> > nonTreeEdges;
	Graph t = Graph::BFSTree(g, NULL, &nonTreeEdges);
	
	vector<Graph> forest = removeBridges(g, t, nonTreeEdges);
	for(size_t i = 0; i < forest.size(); i++) // todo: parallelize this loop
	{
		Graph gPrime = auxiliaryGraph(forest[i]);
		
		set< pair<size_t, size_t> > nonTreeEdgesPrime;
		Graph tPrime = Graph::BFSTreeRooted(gPrime, 0, &nonTreeEdgesPrime);
		vector<Graph> fPrime = removeBridges(gPrime, tPrime, nonTreeEdgesPrime);
		
		for(size_t j = 0; j < fPrime.size(); j++)
		{
			set<size_t> component;
			for(size_t k = 0; k < fPrime[j].V(); k++)
				component.insert(fPrime[j].vertex(k));
			if(component.size() > 1)
				components.insert(component);
		}
	}
	
	return components;
}

int main(int argc, char **argv)
{
	if(argc > 1)
	{
		Graph g(argv[1]);
		cout << "Graph:\n" << g << endl;
		
		set< set<size_t> > components = biconnectedComponents(g);
		cout << "Biconnected components:" << endl;
		for(set< set<size_t> >::iterator i = components.begin(); i != components.end(); ++i)
		{
			for(set<size_t>::iterator j = i->begin(); j != i->end(); ++j)
				cout << char('a' + *j) << " ";
			cout << endl;
		}
	}
	else
	{
		cout << "Usage: ./bicc [inputgraph]" << endl;
	}
	
	return 0;
}
