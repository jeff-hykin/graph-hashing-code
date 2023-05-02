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
#ifndef COLOR_REFINE
   bool isomorphic();
#else
   bool isomorphic();
#endif

   // Vertex partition counts.
   int partitionCount1, partitionCount2;

   // Are graphs equal?
#ifndef COLOR_REFINE
   bool equals();
#else
   bool equals();
#endif
};
#endif
