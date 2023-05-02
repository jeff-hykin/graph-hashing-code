// For conditions of distribution and use, see copyright notice in graphHash.hpp

/*
 * Graph isomorphism.
 */

#ifndef __GRAPH_ISOMORPH__
#define __GRAPH_ISOMORPH__

#include "graphHash.hpp"
#include "random.hpp"
#include <map>

class GraphIsomorph
{
public:

    // Graphs.
    Graph *graph1, *graph2;

    // Constructor.
    GraphIsomorph(Graph *graph1, Graph *graph2);

    // Destructor.
    ~GraphIsomorph();

    // Are graphs isomorphic?
    bool isomorphic();

    // Vertex partition counts.
    int partitionCount1, partitionCount2;

    // Are graphs equal?
    bool equals();
};
#endif
