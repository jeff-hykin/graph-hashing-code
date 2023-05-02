#include "graphHash.hpp"

int graphHashBug2Tester(int argc, char *argv[])
{
   // Create graph.
   Graph *graph = new Graph();

   assert(graph != NULL);

   // Add vertices.
   Graph::Vertex *A = graph->addVertex();
   Graph::Vertex *B = graph->addVertex();
   Graph::Vertex *C = graph->addVertex();
   Graph::Vertex *D = graph->addVertex();
   Graph::Vertex *E = graph->addVertex();
   Graph::Vertex *F = graph->addVertex();
   Graph::Vertex *G = graph->addVertex();
   Graph::Vertex *H = graph->addVertex();
   Graph::Vertex *I = graph->addVertex();
   Graph::Vertex *J = graph->addVertex();
   Graph::Vertex *K = graph->addVertex();
   Graph::Vertex *L = graph->addVertex();
   Graph::Vertex *M = graph->addVertex();
   Graph::Vertex *N = graph->addVertex();
   Graph::Vertex *O = graph->addVertex();
   Graph::Vertex *P = graph->addVertex();
   Graph::Vertex *Q = graph->addVertex();
   Graph::Vertex *R = graph->addVertex();
   Graph::Vertex *S = graph->addVertex();
   Graph::Vertex *T = graph->addVertex();
   Graph::Vertex *U = graph->addVertex();
   Graph::Vertex *V = graph->addVertex();
   Graph::Vertex *W = graph->addVertex();
   Graph::Vertex *X = graph->addVertex();
   Graph::Vertex *Y = graph->addVertex();
   Graph::Vertex *Z = graph->addVertex();
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

   // Connect edges.
   graph->connectVertices(A, B, false);
   graph->connectVertices(B, C, false);
   graph->connectVertices(C, H, false);
   graph->connectVertices(H, I, false);
   graph->connectVertices(I, J, false);
   graph->connectVertices(J, A, false);

   graph->connectVertices(C, D, false);
   graph->connectVertices(D, E, false);
   graph->connectVertices(E, F, false);
   graph->connectVertices(F, G, false);
   graph->connectVertices(G, H, false);

   graph->connectVertices(G, K, false);
   graph->connectVertices(K, L, false);

   graph->connectVertices(L, M, false);
   graph->connectVertices(M, N, false);
   graph->connectVertices(N, O, false);
   graph->connectVertices(O, P, false);
   graph->connectVertices(P, Q, false);
   graph->connectVertices(Q, L, false);

   graph->connectVertices(Q, R, false);
   graph->connectVertices(R, S, false);
   graph->connectVertices(S, T, false);
   graph->connectVertices(T, U, false);
   graph->connectVertices(U, P, false);

   graph->connectVertices(U, V, false);
   graph->connectVertices(V, W, false);
   graph->connectVertices(W, X, false);
   graph->connectVertices(X, O, false);

   graph->connectVertices(X, Y, false);
   graph->connectVertices(Y, d, false);
   graph->connectVertices(d, e, false);
   graph->connectVertices(e, f, false);
   graph->connectVertices(f, W, false);

   graph->connectVertices(Y, Z, false);
   graph->connectVertices(Z, a, false);
   graph->connectVertices(a, b, false);
   graph->connectVertices(b, c, false);
   graph->connectVertices(c, d, false);

   graph->connectVertices(V, l, false);
   graph->connectVertices(l, k, false);
   graph->connectVertices(k, g, false);
   graph->connectVertices(g, f, false);

   graph->connectVertices(l, m, false);
   graph->connectVertices(m, n, false);
   graph->connectVertices(n, o, false);
   graph->connectVertices(o, p, false);
   graph->connectVertices(p, k, false);

   graph->connectVertices(g, h, false);
   graph->connectVertices(h, i, false);
   graph->connectVertices(i, j, false);
   graph->connectVertices(j, e, false);

   if ((argc == 2) && (strcmp(argv[1], "graph1") == 0))
   {
      graph->connectVertices(p, q, false);
      graph->connectVertices(q, r, false);
      graph->connectVertices(r, h, false);
   }
   else if ((argc == 2) && (strcmp(argv[1], "graph2") == 0))
   {
      graph->connectVertices(j, q, false);
      graph->connectVertices(q, r, false);
      graph->connectVertices(r, c, false);
   }
   else
   {
      fprintf(stderr, "Usage: %s \"graph1\" | \"graph2\"\n", argv[0]);
      return(1);
   }

   // Create graph hash.
   GraphHash *hash = new GraphHash();
   assert(hash != NULL);
   hash->hash(graph);

   // Print.
   hash->print();

   return(0);
}
