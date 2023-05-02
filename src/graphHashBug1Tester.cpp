#include "graphHash.hpp"

int graphHashBug1Tester(int argc, char *argv[])
{
   // Create graph.
   Graph *graph = new Graph();

   assert(graph != NULL);

   // Add vertices.
   Graph::Vertex *a = graph->addVertex();
   Graph::Vertex *b = graph->addVertex();
   Graph::Vertex *c = graph->addVertex();
   Graph::Vertex *d = graph->addVertex();
   Graph::Vertex *e = graph->addVertex();
   Graph::Vertex *f = graph->addVertex();
   Graph::Vertex *g = graph->addVertex();
   Graph::Vertex *h = graph->addVertex();
   Graph::Vertex *i = graph->addVertex();
   Graph::Vertex *j = graph->addVertex();
   Graph::Vertex *k = graph->addVertex();
   Graph::Vertex *l = graph->addVertex();
   Graph::Vertex *m = graph->addVertex();
   Graph::Vertex *n = graph->addVertex();
   Graph::Vertex *o = graph->addVertex();
   Graph::Vertex *p = graph->addVertex();
   Graph::Vertex *q = graph->addVertex();
   Graph::Vertex *r = graph->addVertex();
   Graph::Vertex *s = graph->addVertex();
   Graph::Vertex *t = graph->addVertex();
   Graph::Vertex *u = graph->addVertex();
   Graph::Vertex *v = graph->addVertex();
   Graph::Vertex *w = graph->addVertex();
   Graph::Vertex *x = graph->addVertex();
   Graph::Vertex *y = graph->addVertex();
   Graph::Vertex *z = graph->addVertex();

   // Connect edges.
   graph->connectVertices(a, b, false);
   graph->connectVertices(b, d, false);
   graph->connectVertices(d, e, false);
   graph->connectVertices(e, f, false);
   graph->connectVertices(f, c, false);
   graph->connectVertices(c, a, false);

   graph->connectVertices(g, c, false);
   graph->connectVertices(f, j, false);
   graph->connectVertices(j, i, false);
   graph->connectVertices(i, h, false);
   graph->connectVertices(h, g, false);

   graph->connectVertices(e, s, false);
   graph->connectVertices(s, r, false);
   graph->connectVertices(r, l, false);
   graph->connectVertices(l, j, false);

   graph->connectVertices(l, n, false);
   graph->connectVertices(n, m, false);
   graph->connectVertices(m, k, false);
   graph->connectVertices(k, i, false);

   graph->connectVertices(s, t, false);
   graph->connectVertices(t, u, false);
   graph->connectVertices(u, v, false);
   graph->connectVertices(v, q, false);
   graph->connectVertices(q, r, false);

   graph->connectVertices(q, p, false);
   graph->connectVertices(p, o, false);
   graph->connectVertices(o, n, false);

   if ((argc == 2) && (strcmp(argv[1], "graph1") == 0))
   {
      graph->connectVertices(u, z, false);
      graph->connectVertices(z, y, false);
      graph->connectVertices(y, x, false);
      graph->connectVertices(x, w, false);
      graph->connectVertices(w, v, false);
   }
   else if ((argc == 2) && (strcmp(argv[1], "graph2") == 0))
   {
      graph->connectVertices(t, z, false);
      graph->connectVertices(z, y, false);
      graph->connectVertices(y, x, false);
      graph->connectVertices(x, w, false);
      graph->connectVertices(w, u, false);
   }
   else
   {
      fprintf(stderr, "Usage: %s \"graph1\" | \"graph2\"\n", argv[0]);
      return(1);
   }

   // Create graph hash.
   GraphHash *hash = new GraphHash();
   assert(hash != NULL);
#ifndef COLOR_REFINE
   hash->hash(graph, 1);
#else
   hash->hash(graph);
#endif

   // Print.
   hash->print();

   return(0);
}
