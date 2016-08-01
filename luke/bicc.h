#ifndef BICC_H
#define BICC_H

#include <vector>
#include <set>
#include "graph.h"

class BiCC
{
public:
	virtual ~BiCC() {}
	virtual const char *name() = 0;
	virtual void getBiCC(const Graph &g, std::vector< std::set<size_t> > &bicc) = 0;
};

#endif
