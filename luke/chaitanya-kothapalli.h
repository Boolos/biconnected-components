#ifndef CHAITANYA_KOTHAPALLI_H
#define CHAITANYA_KOTHAPALLI_H

#include "bicc.h"

class ChaitanyaKothapalli : public BiCC
{
public:
	virtual const char *name();
	virtual void getBiCC(const Graph &g, std::vector< std::set<size_t> > &bicc);
private:
	void removeBridges(const Graph &g, const Graph &t, const Graph &nt, std::vector<size_t> &parent, std::vector<size_t> &level, std::vector< std::vector<size_t> > &components, std::vector<Edge> *bridges = NULL);
	void auxiliaryGraph(const Graph &g, const Graph &nt, const std::vector<size_t> &parent, const std::vector<size_t> &level, const std::vector<size_t> &component, Graph &aux, std::vector<size_t> &alias);
};

#endif
