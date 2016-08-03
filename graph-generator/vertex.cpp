#include "vertex.hpp"

csce::vertex::vertex(int id) : _id(id) {}

std::string csce::vertex::str() const {
	std::stringstream output;
	output << this->_id;
	return output.str();
}

int csce::vertex::id() const {
	return this->_id;
}