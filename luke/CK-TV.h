#ifndef CK_TV_H
#define CK_TV_H

#include "chaitanya-kothapalli.h"

class CKTV : public ChaitanyaKothapalli
{
public:
	CKTV(size_t nthreads = 1) : ChaitanyaKothapalli(nthreads) {}
	virtual const char *name();
	virtual void getBiCC(const Graph &g, std::vector< std::set<size_t> > &bicc);
};

#endif
