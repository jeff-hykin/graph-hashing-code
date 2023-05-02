/*
 * This software is provided under the terms of the GNU General
 * Public License as published by the Free Software Foundation.
 *
 * Copyright (c) 2007-2016 Tom Portegys, All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for NON-COMMERCIAL purposes and without
 * fee is hereby granted provided that this copyright notice
 * appears in all copies.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.
 */

/*
 * Compute an MD5 hash for a graph.
 */

#ifndef __GRAPH_HASH__
#define __GRAPH_HASH__

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <algorithm>
#include <assert.h>
#ifndef COLOR_REFINE
#ifdef THREADS
#include <pthread.h>
#endif
#endif
#include "graph.hpp"
#include "md5.h"
using namespace std;

class GraphHash
{
public:

   // Graph.
   Graph *graph;

   // Hash code.
   unsigned char code[MD5_SIZE];

   // Constructor.
#ifndef COLOR_REFINE
   GraphHash(bool hashLabels = true);
#else
   GraphHash();
#endif

   // Destructor.
   ~GraphHash();

   // Hash graph.
#ifndef COLOR_REFINE
#ifdef THREADS
   void hash(Graph *graph, int numThreads = 1, bool verbose = false);

#else
   void hash(Graph *graph, bool verbose = false);
#endif
#else
   void hash(Graph *graph);
#endif

#ifndef COLOR_REFINE
   // Hash vertex labels.
   bool hashLabels;
#endif

   // No hash vertex label.
   enum { NO_HASH_VERTEX_LABEL = 0xfffffffe };

   // Get graph hash.
   // Valid after graph hash.
   // Hash size is MD5_SIZE bytes (see md5.h).
   unsigned char *getHash();

   // Print graph and its hash.
   void print(FILE *fp = stdout);

   // Vertex partition count.
   int partitionCount;

   class VertexCoder;

   // Vertex coder link.
   class VertexCoderLink
   {
public:
      Graph::Edge *edge;
      VertexCoder *coder;
      VertexCoderLink();
      VertexCoderLink(Graph::Edge *edge, VertexCoder *coder);
   };

   // Vertex coder.
   class VertexCoder
   {
public:
      Graph::Vertex           *vertex;
      vector<Graph::Vertex *> vertexBranch;
      unsigned char           code[MD5_SIZE];
      bool codeValid;
#ifdef COLOR_REFINE
      unsigned char newcode[MD5_SIZE];
#endif
      vector<VertexCoderLink *> children;
      int labelClass;
      VertexCoder();
      VertexCoder(Graph::Vertex *vertex, vector<Graph::Vertex *> vertexBranch);
      ~VertexCoder();
#ifndef COLOR_REFINE
#ifdef THREADS
      void encode(int numThreads, bool hashLabels, bool verbose);
#endif
      void encode(bool hashLabels, bool verbose);

#else
      void encode();
#endif
      void printCode(FILE *fp = stdout);

private:
#ifndef COLOR_REFINE
      void expand();
      void contract();

#ifdef THREADS
      void encodeVertices(int threadNum, vector<VertexCoder *> *vertexList,
                          bool hashLabels, bool verbose);
      static void *encodeThread(void *threadInfo);
#endif
#endif

      static bool ltcmpCoderLinks(VertexCoderLink *a, VertexCoderLink *b);
      static bool ltcmpCoderLinksLabeled(VertexCoderLink *a, VertexCoderLink *b);
      static int cmpCoderLinks(VertexCoderLink *a, VertexCoderLink *b);

#ifndef COLOR_REFINE
#ifdef THREADS
      int               numThreads;
      bool              terminate;
      pthread_barrier_t updateBarrier;
      pthread_mutex_t   updateMutex;
      pthread_t         *threads;
      struct ThreadInfo
      {
         int                   threadNum;
         VertexCoder           *coder;
         vector<VertexCoder *> *vertexList;
         bool                  hashLabels;
         bool                  verbose;
      };
#endif
#endif
   };

   // Root vertex coder.
   VertexCoder *vertexCoder;

   // Less-than comparison of vertices by label.
   static bool ltcmpVertexLabels(Graph::Vertex *a, Graph::Vertex *b);
};
#endif
