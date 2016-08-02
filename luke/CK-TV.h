#ifndef CK_TV_H
#define CK_TV_H

#include "chaitanya-kothapalli.h"

class CKTV : public ChaitanyaKothapalli
{
public:
	virtual const char *name();
	virtual void getBiCC(const Graph &g, std::vector< std::set<size_t> > &bicc);
};

#endif
