#include "test.hpp"

bool csce::test::run() const {
	std::cout << "running tests ..." << std::endl;
	bool correct = true;
	
	//add any other unit test calls here
	
	std::cout << "tests complete ... " << (correct ? "correct" : "INCORRECT") << std::endl;
	return correct;
}