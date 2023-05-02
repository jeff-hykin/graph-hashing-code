// For conditions of distribution and use, see copyright notice in graphHash.hpp

/*
 *
 * Graph isomorphism tester.
 * Test graph isomorphism algorithm. Prints to standard output.
 *
 */

#ifdef WIN32
#include <windows.h>
#endif
#include <time.h>
#include "graphIsomorph.hpp"
#include "random.hpp"
#include "gettime.h"

// Print usage.
void printUsage(char *arg0)
{
#ifndef COLOR_REFINE
   fprintf(stderr, "Usage: %s\n", arg0);
   fprintf(stderr, "\tGenerate graphs:\n");
   fprintf(stderr, "\t\t-tests <number of graph tests>\n");
   fprintf(stderr, "\t\t-numVertices <number of vertices>\n");
   fprintf(stderr, "\t\t-numEdges <number of edges>\n");
   fprintf(stderr, "\t\t[-directed (directed graph)]\n");
   fprintf(stderr, "\t\t[-structureRandomSeed <graph structure random seed>]\n");
   fprintf(stderr, "\t\t[-isomorphic (generate isomorphic graphs)]\n");
#ifdef THREADS
   fprintf(stderr, "\t\t[-numThreads <number of threads (default=1)>]\n");
#endif
   fprintf(stderr, "\tLoad graphs:\n");
   fprintf(stderr, "\t\t-load1 <graph1 load file> -load2 <graph2 load file>\n");
#ifdef THREADS
   fprintf(stderr, "\t\t[-numThreads <number of threads (default=1)>]\n");
#endif
#else
   fprintf(stderr, "Usage: %s\n", arg0);
   fprintf(stderr, "\tGenerate graphs:\n");
   fprintf(stderr, "\t\t-tests <number of graph tests>\n");
   fprintf(stderr, "\t\t-numVertices <number of vertices>\n");
   fprintf(stderr, "\t\t-numEdges <number of edges>\n");
   fprintf(stderr, "\t\t[-structureRandomSeed <graph structure random seed>]\n");
   fprintf(stderr, "\t\t[-isomorphic (generate isomorphic graphs)]\n");
   fprintf(stderr, "\tLoad graphs:\n");
   fprintf(stderr, "\t\t-load1 <graph1 load file> -load2 <graph2 load file>\n");
#endif
}


// Options.
int    numVertices, numEdges;
bool   directed             = false;
RANDOM structureRandomSeed  = INVALID_RANDOM;
Random *structureRandomizer = NULL;
bool   isomorphic           = false;
char   *loadFile1           = NULL;
char   *loadFile2           = NULL;

// Create graph.
Graph *createGraph();

// Create isomorph of graph.
Graph *isomorphGraph(Graph *source);

// Main.
int main(int argc, char *argv[])
{
   int           i, tests;
   Graph         *graph1, *graph2;
   GraphIsomorph *isomorph;

#ifdef WIN32
   // Direct stdio to parent console.
   if (AttachConsole(ATTACH_PARENT_PROCESS))
   {
      freopen("CONOUT$", "w", stdout);
      freopen("CONOUT$", "w", stderr);
      freopen("CONIN$", "r", stdin);
   }
#endif

   tests = numVertices = numEdges = -1;
#ifdef THREADS
   int numThreads = 1;
#endif
   for (i = 1; i < argc; i++)
   {
      if (strcmp(argv[i], "-tests") == 0)
      {
         i++;
         if (i >= argc)
         {
            printUsage(argv[0]);
            return(1);
         }
         tests = atoi(argv[i]);
         if (tests < 0)
         {
            printUsage(argv[0]);
            return(1);
         }
         continue;
      }

      if (strcmp(argv[i], "-numVertices") == 0)
      {
         i++;
         if (i >= argc)
         {
            printUsage(argv[0]);
            return(1);
         }
         numVertices = atoi(argv[i]);
         if (numVertices < 1)
         {
            printUsage(argv[0]);
            return(1);
         }
         continue;
      }

      if (strcmp(argv[i], "-numEdges") == 0)
      {
         i++;
         if (i >= argc)
         {
            printUsage(argv[0]);
            return(1);
         }
         numEdges = atoi(argv[i]);
         if (numEdges < 0)
         {
            printUsage(argv[0]);
            return(1);
         }
         continue;
      }

      if (strcmp(argv[i], "-directed") == 0)
      {
         directed = true;
         continue;
      }

      if (strcmp(argv[i], "-structureRandomSeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            printUsage(argv[0]);
            return(1);
         }
         structureRandomSeed = (RANDOM)atoi(argv[i]);
         if (structureRandomSeed == INVALID_RANDOM)
         {
            printUsage(argv[0]);
            return(1);
         }
         continue;
      }

      if (strcmp(argv[i], "-isomorphic") == 0)
      {
         isomorphic = true;
         continue;
      }

      if (strcmp(argv[i], "-load1") == 0)
      {
         i++;
         if (i >= argc)
         {
            printUsage(argv[0]);
            return(1);
         }
         loadFile1 = argv[i];
         continue;
      }

      if (strcmp(argv[i], "-load2") == 0)
      {
         i++;
         if (i >= argc)
         {
            printUsage(argv[0]);
            return(1);
         }
         loadFile2 = argv[i];
         continue;
      }

#ifndef COLOR_REFINE
#ifdef THREADS
      if (strcmp(argv[i], "-numThreads") == 0)
      {
         i++;
         if (i >= argc)
         {
            printUsage(argv[0]);
            return(1);
         }
         numThreads = atoi(argv[i]);
         if (numThreads < 1)
         {
            printUsage(argv[0]);
            return(1);
         }
         continue;
      }
#endif
#endif

      printUsage(argv[0]);
      return(1);
   }

   // Check options.
   if ((loadFile1 == NULL) && (loadFile2 == NULL))
   {
      if ((tests == -1) || (numVertices == -1) || (numEdges == -1))
      {
         printUsage(argv[0]);
         return(1);
      }
   }
   else
   {
      if ((loadFile1 == NULL) || (loadFile2 == NULL))
      {
         printUsage(argv[0]);
         return(1);
      }
      if ((tests != -1) || (numVertices != -1) || (numEdges != -1))
      {
         printUsage(argv[0]);
         return(1);
      }
      if (directed || isomorphic)
      {
         printUsage(argv[0]);
         return(1);
      }
   }

   // Load graphs?
   if (loadFile1 != NULL)
   {
      FILE  *fp;
      Graph *graph1 = new Graph();
      assert(graph1 != NULL);
      if ((fp = fopen(loadFile1, "r")) == NULL)
      {
         fprintf(stderr, "%s: Cannot load file %s\n", argv[0], loadFile1);
         return(1);
      }
      graph1->load(fp);
      fclose(fp);
      Graph *graph2 = new Graph();
      assert(graph2 != NULL);
      if ((fp = fopen(loadFile2, "r")) == NULL)
      {
         fprintf(stderr, "%s: Cannot load file %s\n", argv[0], loadFile2);
         return(1);
      }
      graph2->load(fp);
      fclose(fp);

      // Are graphs isomorphic?
      GraphIsomorph *isomorph = new GraphIsomorph(graph1, graph2);
      assert(isomorph != NULL);
#ifndef COLOR_REFINE
#ifdef THREADS
      if (isomorph->isomorphic(numThreads))
#else
      if (isomorph->isomorphic())
#endif
#else
      if (isomorph->isomorphic())
#endif
      {
         printf("Graphs are isomorphic\n");
      }
      else
      {
         printf("Graphs are not isomorphic\n");
      }
      return(0);
   }

   // Create random number generators.
   if (structureRandomSeed == INVALID_RANDOM)
   {
      structureRandomizer = new Random((RANDOM)time(NULL));
   }
   else
   {
      structureRandomizer = new Random(structureRandomSeed);
   }
   assert(structureRandomizer != NULL);

   // Run tests.
   TIME start = gettime();
   for (i = 0; i < tests; i++)
   {
      // Create graphs.
      graph1 = createGraph();
      assert(graph1 != NULL);
      if (isomorphic)
      {
         // Create isomorph.
         graph2 = isomorphGraph(graph1);
      }
      else
      {
         graph2 = createGraph();
      }
      assert(graph2 != NULL);

      // Are graphs isomorphic?
      isomorph = new GraphIsomorph(graph1, graph2);
      assert(isomorph != NULL);
#ifndef COLOR_REFINE
#ifdef THREADS
      if (isomorph->isomorphic(numThreads))
#else
      if (isomorph->isomorphic())
#endif
#else
      if (isomorph->isomorphic())
#endif
      {
         printf("Graphs are isomorphic\n");
      }
      else
      {
         printf("Graphs are not isomorphic\n");
      }
      printf("Hash vertex partition counts=%d/%d\n",
             isomorph->partitionCount1, isomorph->partitionCount2);

      delete isomorph;
      delete graph1;
      delete graph2;
   }
   TIME end = gettime();
   printf("Run time=%lld\n", (end - start));

   return(0);
}


// Create graph.
Graph *createGraph()
{
   int           i;
   Graph::Vertex *v1, *v2;

   Graph *graph = new Graph();

   assert(graph != NULL);

   // Populate graph.
   for (i = 0; i < numVertices; i++)
   {
      graph->addVertex();
   }

   for (i = 0; i < numEdges; i++)
   {
      v1 = graph->vertices[structureRandomizer->RAND_CHOICE((int)graph->vertices.size())];
      v2 = graph->vertices[structureRandomizer->RAND_CHOICE((int)graph->vertices.size())];
      graph->connectVertices(v1, v2, directed);
   }
   return(graph);
}


// Create isomorph of graph.
Graph *isomorphGraph(Graph *source)
{
   int i, i2, j, j2, k;

   vector<Graph::Vertex *> vertices;
   vector<Graph::Edge *>   edges;
   Graph::Vertex           *vertex;

   // Clone and scramble graph.
   Graph *target = source->clone();
   i2 = (int)target->vertices.size();
   if (i2 > 0)
   {
      k = structureRandomizer->RAND_CHOICE(i2);
      for (i = 0; i < i2; i++)
      {
         vertices.push_back(target->vertices[k]);
         k = (k + 1) % i2;
      }
      target->vertices.clear();
      for (i = 0; i < i2; i++)
      {
         target->vertices.push_back(vertices[i]);
      }
      for (i = 0; i < i2; i++)
      {
         vertex = target->vertices[i];
         j2     = (int)vertex->edges.size();
         if (j2 > 0)
         {
            k = structureRandomizer->RAND_CHOICE(j2);
            for (j = 0; j < j2; j++)
            {
               edges.push_back(vertex->edges[k]);
               k = (k + 1) % j2;
            }
            vertex->edges.clear();
            for (j = 0; j < j2; j++)
            {
               vertex->edges.push_back(edges[j]);
            }
            edges.clear();
         }
      }
   }
   return(target);
}
