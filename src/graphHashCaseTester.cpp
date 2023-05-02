// https://cacm.acm.org/magazines/2020/11/248220-the-graph-isomorphism-problem/fulltext

#include "graphHash.hpp"
#include "graphIsomorph.hpp"

int graphHashCaseTester(int argc, char *argv[])
{
   // Figure 5.
   Graph *graph1 = new Graph();
   assert(graph1 != NULL);
   Graph::Vertex *One   = graph1->addVertex();
   Graph::Vertex *Two   = graph1->addVertex();
   Graph::Vertex *Three = graph1->addVertex();
   Graph::Vertex *Four  = graph1->addVertex();
   Graph::Vertex *Five  = graph1->addVertex();
   Graph::Vertex *Six   = graph1->addVertex();
   graph1->connectVertices(One, Two, false);
   graph1->connectVertices(One, Three, false);
   graph1->connectVertices(Two, Three, false);
   graph1->connectVertices(Three, Four, false);
   graph1->connectVertices(Four, Five, false);
   graph1->connectVertices(Four, Six, false);
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
   graph2->connectVertices(One, Four, false);
   graph2->connectVertices(Two, Three, false);
   graph2->connectVertices(Three, Four, false);
   graph2->connectVertices(Three, Five, false);
   graph2->connectVertices(Four, Six, false);
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

   // Figure 2.
   graph1 = new Graph();
   assert(graph1 != NULL);
   One = graph1->addVertex();
   Two = graph1->addVertex();
   Three = graph1->addVertex();
   Four = graph1->addVertex();
   Five = graph1->addVertex();
   Six = graph1->addVertex();
   Graph::Vertex* Seven = graph1->addVertex();
   Graph::Vertex* Eight = graph1->addVertex();
   graph1->connectVertices(One, Two, false);
   graph1->connectVertices(One, Three, false);
   graph1->connectVertices(One, Eight, false);
   graph1->connectVertices(Two, Four, false);
   graph1->connectVertices(Two, Seven, false);
   graph1->connectVertices(Three, Five, false);
   graph1->connectVertices(Three, Six, false);
   graph1->connectVertices(Four, Six, false);
   graph1->connectVertices(Four, Five, false);
   graph1->connectVertices(Five, Seven, false);
   graph1->connectVertices(Six, Eight, false);
   graph1->connectVertices(Seven, Eight, false);

   graph2 = new Graph();
   assert(graph2 != NULL);
   One = graph2->addVertex();
   Two = graph2->addVertex();
   Three = graph2->addVertex();
   Four = graph2->addVertex();
   Five = graph2->addVertex();
   Six = graph2->addVertex();
   Seven = graph2->addVertex();
   Eight = graph2->addVertex();
   graph2->connectVertices(One, Two, false);
   graph2->connectVertices(One, Five, false);
   graph2->connectVertices(Two, Three, false);
   graph2->connectVertices(Two, Six, false);
   graph2->connectVertices(Three, Four, false);
   graph2->connectVertices(Three, Seven, false);
   graph2->connectVertices(Four, Eight, false);
   graph2->connectVertices(Five, Six, false);
   graph2->connectVertices(Six, Seven, false);
   graph2->connectVertices(Seven, Eight, false);
   graph2->connectVertices(Four, Five, false);
   graph2->connectVertices(One, Eight, false);

   hash = new GraphHash();
   assert(hash != NULL);
   hash->hash(graph1);
   hash->print();

   hash = new GraphHash();
   assert(hash != NULL);
   hash->hash(graph2);
   hash->print();

   isomorph = new GraphIsomorph(graph1, graph2);
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
