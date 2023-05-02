// For conditions of distribution and use, see copyright notice in graphHash.hpp

/*
 *
 * Graph hash driver.
 *
 */

#ifdef WIN32
#include <windows.h>
#endif
#include <time.h>
#include "graphHash.hpp"
#include "random.hpp"
#include "gettime.h"

// Print usage.
void printUsage(char *arg0)
{
   fprintf(stderr, "Usage: %s\n", arg0);
   fprintf(stderr, "\tCreate graph and hash:\n");
   fprintf(stderr, "\t\t-numVertices <number of vertices>\n");
   fprintf(stderr, "\t\t-numEdges <number of edges>\n");
   fprintf(stderr, "\t\t[-labelVertices <none (default) | unique | random>]\n");
   fprintf(stderr, "\t\t[-labelEdges <none (default) | unique | random>]\n");
   fprintf(stderr, "\t\t[-directed (directed graph)]\n");
   fprintf(stderr, "\t\t[-structureRandomSeed <graph structure random seed>]\n");
   fprintf(stderr, "\t\t[-labelRandomSeed <label random seed>]\n");
   fprintf(stderr, "\t\t[-save <graph save file>]\n");
#ifndef COLOR_REFINE
   fprintf(stderr, "\t\t[-verbose]\n");
#ifdef THREADS
   fprintf(stderr, "\t\t[-numThreads <number of threads (default=1)>]\n");
#endif
#endif
   fprintf(stderr, "\tLoad graph and create hash:\n");
   fprintf(stderr, "\t\t[-load <graph load file>]\n");
#ifndef COLOR_REFINE
   fprintf(stderr, "\t\t[-verbose]\n");
#ifdef THREADS
   fprintf(stderr, "\t\t[-numThreads <number of threads (default=1)>]\n");
#endif
#endif
}


// Main.
int main(int argc, char *argv[])
{
   int i;
   int numVertices, numEdges;

   typedef enum
   {
      NO_LABEL, UNIQUE_LABEL, RANDOM_LABEL
   }
   LABEL_TYPE;
   LABEL_TYPE    vertexLabel = NO_LABEL;
   LABEL_TYPE    edgeLabel   = NO_LABEL;
   bool          gotLabel;
   bool          directed;
   RANDOM        structureRandomSeed  = INVALID_RANDOM;
   RANDOM        labelRandomSeed      = INVALID_RANDOM;
   Random        *structureRandomizer = NULL;
   Random        *labelRandomizer     = NULL;
   Graph::Vertex *v1, *v2;
   char          *loadFile, *saveFile;
#ifndef COLOR_REFINE
   bool verbose;
#endif
   FILE *fp;

#ifdef WIN32
   // Direct stdio to parent console.
   if (AttachConsole(ATTACH_PARENT_PROCESS))
   {
      freopen("CONOUT$", "w", stdout);
      freopen("CONOUT$", "w", stderr);
      freopen("CONIN$", "r", stdin);
   }
#endif

   numVertices = numEdges = -1;
   gotLabel    = false;
   directed    = false;
   loadFile    = saveFile = NULL;
#ifndef COLOR_REFINE
   verbose = false;
#ifdef THREADS
   int numThreads = 1;
#endif
#endif
   for (i = 1; i < argc; i++)
   {
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

      if (strcmp(argv[i], "-labelVertices") == 0)
      {
         i++;
         if (i >= argc)
         {
            printUsage(argv[0]);
            return(1);
         }
         if (strcmp(argv[i], "none") == 0)
         {
            vertexLabel = NO_LABEL;
         }
         else if (strcmp(argv[i], "unique") == 0)
         {
            vertexLabel = UNIQUE_LABEL;
         }
         else if (strcmp(argv[i], "random") == 0)
         {
            vertexLabel = RANDOM_LABEL;
         }
         else
         {
            printUsage(argv[0]);
            return(1);
         }
         gotLabel = true;
         continue;
      }

      if (strcmp(argv[i], "-labelEdges") == 0)
      {
         i++;
         if (i >= argc)
         {
            printUsage(argv[0]);
            return(1);
         }
         if (strcmp(argv[i], "none") == 0)
         {
            edgeLabel = NO_LABEL;
         }
         else if (strcmp(argv[i], "unique") == 0)
         {
            edgeLabel = UNIQUE_LABEL;
         }
         else if (strcmp(argv[i], "random") == 0)
         {
            edgeLabel = RANDOM_LABEL;
         }
         else
         {
            printUsage(argv[0]);
            return(1);
         }
         gotLabel = true;
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

      if (strcmp(argv[i], "-labelRandomSeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            printUsage(argv[0]);
            return(1);
         }
         labelRandomSeed = (RANDOM)atoi(argv[i]);
         if (labelRandomSeed == INVALID_RANDOM)
         {
            printUsage(argv[0]);
            return(1);
         }
         continue;
      }

      if (strcmp(argv[i], "-save") == 0)
      {
         i++;
         if (i >= argc)
         {
            printUsage(argv[0]);
            return(1);
         }
         saveFile = argv[i];
         continue;
      }

      if (strcmp(argv[i], "-load") == 0)
      {
         i++;
         if (i >= argc)
         {
            printUsage(argv[0]);
            return(1);
         }
         loadFile = argv[i];
         continue;
      }

#ifndef COLOR_REFINE
      if (strcmp(argv[i], "-verbose") == 0)
      {
         verbose = true;
         continue;
      }

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
   if (loadFile == NULL)
   {
      if ((numVertices == -1) || (numEdges == -1))
      {
         printUsage(argv[0]);
         return(1);
      }
   }
   else
   {
      if ((numVertices != -1) || (numEdges != -1) || gotLabel ||
          directed || (structureRandomSeed != INVALID_RANDOM) ||
          (labelRandomSeed != INVALID_RANDOM) || (saveFile != NULL))
      {
         printUsage(argv[0]);
         return(1);
      }
   }

   // Create graph.
   Graph *graph = new Graph();
   assert(graph != NULL);

   // Load graph?
   if (loadFile != NULL)
   {
      if ((fp = fopen(loadFile, "r")) == NULL)
      {
         fprintf(stderr, "%s: Cannot load file %s\n", argv[0], loadFile);
         return(1);
      }
      graph->load(fp);
      fclose(fp);
   }                                              // Create graph.
   else
   {
      // Create random number generators.
      if (structureRandomSeed == INVALID_RANDOM)
      {
         structureRandomSeed = (RANDOM)time(NULL);
      }
      structureRandomizer = new Random(structureRandomSeed);
      assert(structureRandomizer != NULL);
      if (labelRandomSeed == INVALID_RANDOM)
      {
         labelRandomSeed = (RANDOM)time(NULL);
      }
      labelRandomizer = new Random(labelRandomSeed);
      assert(labelRandomizer != NULL);

      // Populate graph.
      for (i = 0; i < numVertices; i++)
      {
         switch (vertexLabel)
         {
         case NO_LABEL:
            graph->addVertex();
            break;

         case UNIQUE_LABEL:
            graph->addVertex((unsigned short)i);
            break;

         case RANDOM_LABEL:
            graph->addVertex((unsigned short)labelRandomizer->RAND_CHOICE(numVertices));
            break;
         }
      }
      for (i = 0; i < numEdges; i++)
      {
         v1 = graph->vertices[structureRandomizer->RAND_CHOICE((int)graph->vertices.size())];
         v2 = graph->vertices[structureRandomizer->RAND_CHOICE((int)graph->vertices.size())];
         switch (edgeLabel)
         {
         case NO_LABEL:
            graph->connectVertices(v1, v2, directed);
            break;

         case UNIQUE_LABEL:
            graph->connectVertices(v1, v2, directed, i);
            break;

         case RANDOM_LABEL:
            graph->connectVertices(v1, v2, directed, labelRandomizer->RAND_CHOICE(numEdges));
            break;
         }
      }
   }

   // Create graph hasher.
   GraphHash *hash = new GraphHash();
   assert(hash != NULL);

   // Hash graph.
#ifndef COLOR_REFINE
   if (verbose)
   {
#ifdef WIN32
      SYSTEMTIME t;
      char       buffer[1024];
      GetLocalTime(&t);
      if (GetDateFormat(LOCALE_USER_DEFAULT,
                        0,
                        &t,
                        "dd MMM yyyy",
                        (LPTSTR)buffer,
                        sizeof(buffer)) > 0)
      {
         printf("Begin hash at: %s ", buffer);
      }
      if (GetTimeFormat(LOCALE_USER_DEFAULT,
                        0,
                        &t,
                        "hh:mm:ss",
                        (LPTSTR)buffer,
                        sizeof(buffer)) > 0)
      {
         printf("%s\n", buffer);
      }
#else
      time_t t = time(NULL);
      printf("Begin hash at: %s", ctime(&t));
#endif
   }
   clock_t t = clock();

#ifdef THREADS
   hash->hash(graph, numThreads, verbose);
#else
   hash->hash(graph, verbose);
#endif

   if (verbose)
   {
      t = clock() - t;
      double time_taken = ((double)t) / CLOCKS_PER_SEC;
      printf("Hash time=%f seconds\n", time_taken);
   }
#else
   hash->hash(graph);
#endif

   // Print.
   hash->print();

   // Save graph?
   if (saveFile != NULL)
   {
      if ((fp = fopen(saveFile, "w")) == NULL)
      {
         fprintf(stderr, "%s: Cannot save file %s\n", argv[0], saveFile);
         return(1);
      }
      graph->save(fp);
      fclose(fp);
   }

   return(0);
}
