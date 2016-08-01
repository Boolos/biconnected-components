#include "graph.hpp"

using namespace std;

int main(int argc, char **argv)
{
    csce::Graph testGraph;
    testGraph.add(0,1).add(0,5).add(0,6)
        .add(1,2).add(1,3).add(1,4).add(1,0)
        .add(2,3).add(2,1)
        .add(3,2).add(3,1)
        .add(4,1).add(4,5)
        .add(5,4).add(5,0)
        .add(6,0).add(6,8).add(6,7)
        .add(7,6).add(7,8)
        .add(8,6).add(8,7);

    vector<csce::Graph> biconnectedComponents;
    
    csce::Graph comp1;
    comp1.add(1,2).add(2,3).add(3,1);
    biconnectedComponents.push_back(comp1);
    
    csce::Graph comp2;
    comp2.add(1,4).add(4,5).add(5,0).add(0,1);
    biconnectedComponents.push_back(comp2);

    csce::Graph comp3;
    comp3.add(0,6);
    biconnectedComponents.push_back(comp3);

    csce::Graph comp4;
    comp4.add(6,8).add(8,7).add(7,6);
    biconnectedComponents.push_back(comp4);

    vector<size_t> articulationPoins;
    articulationPoins.push_back(0);
    articulationPoins.push_back(1);
    articulationPoins.push_back(6);
    
    // run your algorithm and test it against the biconnectedComponents above
    // your result should be the exact same number of components
    // you can just use the "==" operator to check if two Graph components are the same

	return 0;
}

