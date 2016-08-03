#ifndef vertex_hpp
#define vertex_hpp

#include <iostream>
#include <sstream>

namespace csce {
	class vertex {
	public:
		vertex(int id);
		bool operator==(const csce::vertex& other) const {
			return _id == other._id;
		}
		
		bool operator!=(const csce::vertex& other) const {
			return !(*this == other);
		}
		
		bool operator<(const csce::vertex& other) const {
			return this->_id < other._id;
		}
		
		friend std::ostream& operator<<(std::ostream& stream, const csce::vertex& v) {
			stream << v.str();
			return stream;
		}
		
		std::string str() const;
		int id() const;
		
	private:
		int _id;
	};
}

namespace std {
	template<> struct hash<csce::vertex> {
		std::size_t operator()(const csce::vertex& v) const {
			return std::hash<int>()(v.id());
		}
	};
}

#endif /* vertex_hpp */