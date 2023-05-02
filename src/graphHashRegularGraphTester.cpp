#include "graphHash.hpp"
#include "graphIsomorph.hpp"

int graphHashRegularGraphTester(int argc, char *argv[])
{
   Graph *graph1 = new Graph();

   assert(graph1 != NULL);
   Graph::Vertex *One   = graph1->addVertex();
   Graph::Vertex *Two   = graph1->addVertex();
   Graph::Vertex *Three = graph1->addVertex();
   Graph::Vertex *Four  = graph1->addVertex();
   Graph::Vertex *Five  = graph1->addVertex();
   Graph::Vertex *Six   = graph1->addVertex();
   graph1->connectVertices(One, Two, false);
   graph1->connectVertices(One, Four, false);
   graph1->connectVertices(One, Six, false);
   graph1->connectVertices(Two, Three, false);
   graph1->connectVertices(Two, Five, false);
   graph1->connectVertices(Three, Four, false);
   graph1->connectVertices(Three, Six, false);
   graph1->connectVertices(Four, Five, false);
   graph1->connectVertices(Five, Six, false);

   Graph *graph2 = new Graph();
   assert(graph2 != NULL);
   One   = graph2->addVertex();
   Two   = graph2->addVertex();
   Three = graph2->addVertex();
   Four  = graph2->addVertex();
   Five  = graph2->addVertex();
   Six   = graph2->addVertex();
   graph2->connectVertices(One, Two, false);
   graph2->connectVertices(One, Five, false);
   graph2->connectVertices(One, Six, false);
   graph2->connectVertices(Two, Three, false);
   graph2->connectVertices(Two, Four, false);
   graph2->connectVertices(Three, Four, false);
   graph2->connectVertices(Three, Six, false);
   graph2->connectVertices(Four, Five, false);
   graph2->connectVertices(Five, Six, false);

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
