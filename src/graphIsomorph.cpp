// For conditions of distribution and use, see copyright notice in graphHash.hpp

/*
 * Graph isomorphism.
 */

#include "graphIsomorph.hpp"

// Constructor.
GraphIsomorph::GraphIsomorph(Graph *graph1, Graph *graph2)
{
   this->graph1    = graph1;
   this->graph2    = graph2;
   partitionCount1 = 0;
   partitionCount2 = 0;
}


// Destructor.
GraphIsomorph::~GraphIsomorph()
{
}


// Are graphs isomorphic?
#ifndef COLOR_REFINE
#ifdef THREADS
bool GraphIsomorph::isomorphic(int numThreads)
#else
bool GraphIsomorph::isomorphic()
#endif
#else
bool GraphIsomorph::isomorphic()
#endif
{
   int i, i2, j, j2, d1, u1, d2, u2;

   // Graphs must have equal numbers of vertices and edges.
   if (graph1->vertices.size() != graph2->vertices.size())
   {
      return(false);
   }
   if (graph1->vertices.empty())
   {
      return(true);
   }
   d1 = u1 = d2 = u2 = 0;
   for (i = 0, i2 = (int)graph1->vertices.size(); i < i2; i++)
   {
      for (j = 0, j2 = (int)graph1->vertices[i]->edges.size(); j < j2; j++)
      {
         if (graph1->vertices[i]->edges[j]->directed)
         {
            if (graph1->vertices[i]->edges[j]->source == graph1->vertices[i])
            {
               d1++;
            }
         }
         else
         {
            u1++;
         }
      }
      for (j = 0, j2 = (int)graph2->vertices[i]->edges.size(); j < j2; j++)
      {
         if (graph2->vertices[i]->edges[j]->directed)
         {
            if (graph2->vertices[i]->edges[j]->source == graph2->vertices[i])
            {
               d2++;
            }
         }
         else
         {
            u2++;
         }
      }
   }
   if ((d1 != d2) || (u1 != u2))
   {
      return(false);
   }

   // Hash graphs without labels.
   GraphHash *hash1 = new GraphHash();
   assert(hash1 != NULL);
#ifndef COLOR_REFINE
#ifdef THREADS
   hash1->hash(graph1, numThreads, false);
#else
   hash1->hash(graph1, false);
#endif
#else
   hash1->hash(graph1);
#endif
   partitionCount1 = hash1->partitionCount;

   GraphHash *hash2 = new GraphHash();
   assert(hash2 != NULL);
#ifndef COLOR_REFINE
#ifdef THREADS
   hash2->hash(graph2, numThreads, false);
#else
   hash2->hash(graph2, false);
#endif
#else
   hash2->hash(graph2);
#endif
   partitionCount2 = hash2->partitionCount;

   // Graph structures differ?
   bool result;
   if (memcmp(hash1->code, hash2->code, MD5_SIZE) != 0)
   {
      result = false;
   }
   else
   {
      result = true;
   }
   delete hash1;
   delete hash2;
   return(result);
}


// Are graphs equal?
#ifndef COLOR_REFINE
#ifdef THREADS
bool GraphIsomorph::equals(int numThreads)
#else
bool GraphIsomorph::equals()
#endif
#else
bool GraphIsomorph::equals()
#endif
{
   int  i, i2, j, j2, d1, u1, d2, u2;
   bool result;

   // Graphs must have equal numbers of vertices and edges.
   if (graph1->vertices.size() != graph2->vertices.size())
   {
      return(false);
   }
   if (graph1->vertices.empty())
   {
      return(true);
   }
   d1 = u1 = d2 = u2 = 0;
   for (i = 0, i2 = (int)graph1->vertices.size(); i < i2; i++)
   {
      for (j = 0, j2 = (int)graph1->vertices[i]->edges.size(); j < j2; j++)
      {
         if (graph1->vertices[i]->edges[j]->directed)
         {
            if (graph1->vertices[i]->edges[j]->source == graph1->vertices[i])
            {
               d1++;
            }
         }
         else
         {
            u1++;
         }
      }
      for (j = 0, j2 = (int)graph2->vertices[i]->edges.size(); j < j2; j++)
      {
         if (graph2->vertices[i]->edges[j]->directed)
         {
            if (graph2->vertices[i]->edges[j]->source == graph2->vertices[i])
            {
               d2++;
            }
         }
         else
         {
            u2++;
         }
      }
   }
   if ((d1 != d2) || (u1 != u2))
   {
      return(false);
   }

   // Hash graphs with labels.
   GraphHash *hash1 = new GraphHash();
   assert(hash1 != NULL);
#ifndef COLOR_REFINE
#ifdef THREADS
   hash1->hash(graph1, numThreads, true);
#else
   hash1->hash(graph1, true);
#endif
#else
   hash1->hash(graph1);
#endif
   GraphHash *hash2 = new GraphHash();
   assert(hash2 != NULL);
#ifndef COLOR_REFINE
#ifdef THREADS
   hash2->hash(graph2, numThreads, true);
#else
   hash2->hash(graph2, true);
#endif
#else
   hash2->hash(graph2);
#endif

   // Graph hashes equal?
   if (memcmp(hash1->code, hash2->code, MD5_SIZE) == 0)
   {
      result = true;
   }
   else
   {
      result = false;
   }
   delete hash1;
   delete hash2;
   return(result);
}
