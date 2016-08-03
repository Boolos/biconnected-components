#ifndef test_hpp
#define test_hpp

#include <iostream>
#include <vector>

#include "utility.hpp"

namespace csce {
	class test {
	public:
		test(bool _debug = false) : debug(_debug) {}
		bool run() const;
		
	private:
		bool debug = false;
	};
}

#endif /* test_hpp */
