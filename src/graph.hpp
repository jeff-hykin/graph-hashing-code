// For conditions of distribution and use, see copyright notice in graphHash.hpp

/*
 * Graph.
 * The graph can have labeled or non-labeled vertices and edges.
 * An undirected graph contains a pair of bi-directional edges
 * between all connected vertices.
 */

#ifndef __GRAPH__
#define __GRAPH__

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <assert.h>
#include "fileio.h"
using namespace std;

class Graph
{
public:

   // Null label.
   enum { NULL_LABEL=0xffffffff };

   class Edge;

   // Graph vertex.
   class Vertex
   {
public:
      unsigned int   id;
      unsigned int   label;
      vector<Edge *> edges;
      Vertex(unsigned int label = NULL_LABEL);
      void sourceEdges(vector<Edge *>& edgeList);
   };

   // Graph edge.
   class Edge
   {
public:
      unsigned int label;
      Vertex       *source;
      Vertex       *target;
      bool         directed;
      Edge(unsigned int label = NULL_LABEL);
   };

   // Vertices.
   vector<Vertex *> vertices;

   // Constructor.
   Graph();

   // Destructor.
   ~Graph();

   // Add vertex.
   Vertex *addVertex(unsigned int label = NULL_LABEL);

   // Connect vertices.
   Edge *connectVertices(Vertex *source, Vertex *target,
                         bool directed, unsigned int label = NULL_LABEL);

   // Get vertex by label: returns first found.
   Vertex *getVertex(unsigned int label);

   // Clear.
   void clear();

   // Clone.
   Graph *clone();

   // Remove labeled edges by transferring labels to extra vertices.
   Graph *labeledEdges2Vertices();

   // Identify vertices.
   void id();

   // Load and save.
   void load(FILE *fp);
   void save(FILE *fp);

   // Print.
   void print(FILE *fp = stdout);
   void print(char *label, FILE *fp = stdout);

   // Dump graph in Graphviz "dot" format.
   void dump(FILE *fp = stdout);
   void dump(char *label, FILE *fp = stdout);
   void dumpSub(FILE *fp = stdout);
};
#endif
