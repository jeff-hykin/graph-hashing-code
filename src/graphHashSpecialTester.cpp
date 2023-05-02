// Graph hash special case tester.

#include "graphIsomorph.hpp"
#include "graph_ge.h"

// Special cases.
int graphHashBug1Tester(int argc, char *argv[]);
int graphHashBug2Tester(int argc, char *argv[]);
int graphHashRegularGraphTester(int argc, char *argv[]);
int graphHashCaseTester(int argc, char* argv[]);

void printUsage(char *arg0)
{
   fprintf(stderr, "Usage: %s\n", arg0);
   fprintf(stderr, "\tgraphHashBug1Tester \"graph1\" | \"graph2\"\n");
   fprintf(stderr, "|");
   fprintf(stderr, "\tgraphHashBug2Tester \"graph1\" | \"graph2\"\n");
   fprintf(stderr, "|");
   fprintf(stderr, "\tgraphHashRegularGraphTester\n");
   fprintf(stderr, "|");
   fprintf(stderr, "\tgraphHashCaseTester\n");
   fprintf(stderr, "|");
   fprintf(stderr, "\tgraph_ge <random seed> <number of vertices> <vertex degree>\n");
}


int main(int argc, char *argv[])
{
   if (argc > 1)
   {
      if (strcmp(argv[1], "graphHashBug1Tester") == 0)
      {
         return(graphHashBug1Tester(argc - 1, &argv[1]));
      }
      else if (strcmp(argv[1], "graphHashBug2Tester") == 0)
      {
         return(graphHashBug2Tester(argc - 1, &argv[1]));
      }
      else if (strcmp(argv[1], "graphHashRegularGraphTester") == 0)
      {
          return(graphHashRegularGraphTester(argc - 1, &argv[1]));
      }
      else if (strcmp(argv[1], "graphHashCaseTester") == 0)
      {
          return(graphHashCaseTester(argc - 1, &argv[1]));
      }
      else if (strcmp(argv[1], "graph_ge") == 0)
      {
         if (argc == 5)
         {
            int   rand_seed = atoi(argv[2]);
            int   vertices  = atoi(argv[3]);
            int   degree    = atoi(argv[4]);
            Graph *graph1;
            Graph *graph2;
            seed_ran(rand_seed);
            regular_graph(vertices, degree, &graph1);
            regular_graph(vertices, degree, &graph2);
            graph1->dump();
            graph2->dump();

            GraphHash *hash = new GraphHash();
            assert(hash != NULL);
            hash->hash(graph1);
            hash->print();

            hash = new GraphHash();
            assert(hash != NULL);
            hash->hash(graph2);
            hash->print();

            GraphIsomorph *isomorph = new GraphIsomorph(graph1, graph2);
            if (isomorph->isomorphic())
            {
               printf("isomorphic\n");
            }
            else
            {
               printf("not isomorphic\n");
            }
            printf("Hash vertex partition counts=%d/%d\n",
                   isomorph->partitionCount1, isomorph->partitionCount2);
            return(0);
         }
      }
   }
   printUsage(argv[0]);
   return(1);
}
