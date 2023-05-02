// For conditions of distribution and use, see copyright notice in graphHash.hpp

/*
 * Graph.
 */

#include "graph.hpp"

// Constructor.
Graph::Graph()
{
}


// Destructor.
Graph::~Graph()
{
   clear();
}


// Add vertex.
Graph::Vertex *Graph::addVertex(unsigned int label)
{
   Vertex *vertex = new Vertex(label);

   assert(vertex != NULL);
   vertices.push_back(vertex);
   return(vertex);
}


// Connect vertices.
Graph::Edge *Graph::connectVertices(Vertex *source, Vertex *target,
                                    bool directed, unsigned int label)
{
   Edge *edge = new Edge();

   assert(edge != NULL);
   edge->source   = source;
   edge->target   = target;
   edge->directed = directed;
   edge->label    = label;
   source->edges.push_back(edge);
   if (source != target)
   {
      target->edges.push_back(edge);
   }
   return(edge);
}


// Get vertex by label: returns first found.
Graph::Vertex *Graph::getVertex(unsigned int label)
{
   int i, i2;

   for (i = 0, i2 = (int)vertices.size(); i < i2; i++)
   {
      if (vertices[i]->label == label)
      {
         return(vertices[i]);
      }
   }
   return(NULL);
}


// Clear graph.
void Graph::clear()
{
   int i, i2, j, j2;

   vector<Edge *> edgeList;

   for (i = 0, i2 = (int)vertices.size(); i < i2; i++)
   {
      for (j = 0, j2 = (int)vertices[i]->edges.size(); j < j2; j++)
      {
         if (vertices[i]->edges[j]->source == vertices[i])
         {
            edgeList.push_back(vertices[i]->edges[j]);
         }
      }
   }
   for (i = 0, i2 = (int)edgeList.size(); i < i2; i++)
   {
      delete edgeList[i];
   }
   for (i = 0, i2 = (int)vertices.size(); i < i2; i++)
   {
      delete vertices[i];
   }
   vertices.clear();
}


// Clone graph.
Graph *Graph::clone()
{
   int    i, i2, j, j2, k;
   Vertex *vertex, *source, *target;
   Edge   *edge;

   vector<Edge *> edgeList;
   Graph          *graph;

   graph = new Graph();
   assert(graph != NULL);
   for (i = 0, i2 = (int)vertices.size(); i < i2; i++)
   {
      vertex     = graph->addVertex(vertices[i]->label);
      vertex->id = vertices[i]->id;
   }
   for (i = 0; i < i2; i++)
   {
      vertex = vertices[i];
      vertex->sourceEdges(edgeList);
      j2 = (int)edgeList.size();
      for (j = 0; j < j2; j++)
      {
         edge   = edgeList[j];
         source = graph->vertices[i];
         for (k = 0; k < i2; k++)
         {
            if (vertices[k] == edge->target)
            {
               break;
            }
         }
         target = graph->vertices[k];
         graph->connectVertices(source, target, edge->directed, edge->label);
      }
   }
   return(graph);
}


// Remove labeled edges by transferring labels to extra vertices.
Graph *Graph::labeledEdges2Vertices()
{
   int           i, i2, j, j2;
   Graph::Vertex *v, *v2;
   Graph::Edge   *e, *e2;

   vector<Graph::Edge *>::iterator eItr;
   vector<Graph::Vertex *>         vertexList;

   Graph *g = clone();
   for (i = 0, i2 = (int)g->vertices.size(); i < i2; i++)
   {
      v  = g->vertices[i];
      j2 = (int)v->edges.size();
      for (j = 0; j < j2; j++)
      {
         e = v->edges[j];
         if ((e->source == v) && (e->label != Graph::NULL_LABEL))
         {
            v2 = new Graph::Vertex(e->label);
            assert(v2 != NULL);
            vertexList.push_back(v2);
            e->label = Graph::NULL_LABEL;
            e2       = new Graph::Edge();
            assert(e2 != NULL);
            e2->source   = v2;
            e2->target   = e->target;
            e2->directed = e->directed;
            e->target    = v2;
            v2->edges.push_back(e);
            v2->edges.push_back(e2);
            v2 = e2->target;
            if (v2 != v)
            {
               for (eItr = v2->edges.begin(); eItr != v2->edges.end(); ++eItr)
               {
                  if (*eItr == e)
                  {
                     v2->edges.erase(eItr);
                     break;
                  }
               }
            }
            v2->edges.push_back(e2);
         }
      }
   }
   for (i = 0, i2 = (int)vertexList.size(); i < i2; i++)
   {
      g->vertices.push_back(vertexList[i]);
   }
   vertexList.clear();

   return(g);
}


// Identify graph vertices.
void Graph::id()
{
   int i, i2;

   for (i = 0, i2 = (int)vertices.size(); i < i2; i++)
   {
      vertices[i]->id = i;
   }
}


// Load graph.
void Graph::load(FILE *fp)
{
   int          i, i2, j, j2, k, id;
   unsigned int label;
   Vertex       *vertex;
   Edge         *edge;

   clear();
   FREAD_INT(&i2, fp);
   for (i = 0; i < i2; i++)
   {
      FREAD_INT((int *)&id, fp);
      FREAD_INT((int *)&label, fp);
      vertex = new Vertex(label);
      assert(vertex != NULL);
      vertex->id = id;
      vertices.push_back(vertex);
   }
   for (i = 0; i < i2; i++)
   {
      vertex = vertices[i];
      FREAD_INT(&j2, fp);
      for (j = 0; j < j2; j++)
      {
         FREAD_INT((int *)&label, fp);
         edge = new Edge(label);
         assert(edge != NULL);
         edge->source = vertex;
         FREAD_INT(&k, fp);
         edge->target = vertices[k];
         FREAD_BOOL(&edge->directed, fp);
         vertex->edges.push_back(edge);
         if (k != i)
         {
            vertices[k]->edges.push_back(edge);
         }
      }
   }
}


// Save graph.
void Graph::save(FILE *fp)
{
   int i, i2, j, j2, k;

   vector<Edge *> edgeList;
   Vertex         *vertex;
   Edge           *edge;

   i2 = (int)vertices.size();
   FWRITE_INT(&i2, fp);
   for (i = 0; i < i2; i++)
   {
      vertex = vertices[i];
      FWRITE_INT((int *)&vertex->id, fp);
      FWRITE_INT((int *)&vertex->label, fp);
   }
   for (i = 0; i < i2; i++)
   {
      vertex = vertices[i];
      vertex->sourceEdges(edgeList);
      j2 = (int)edgeList.size();
      FWRITE_INT(&j2, fp);
      for (j = 0; j < j2; j++)
      {
         edge = edgeList[j];
         FWRITE_INT((int *)&edge->label, fp);
         for (k = 0; k < i2; k++)
         {
            if (vertices[k] == edge->target)
            {
               break;
            }
         }
         FWRITE_INT(&k, fp);
         FWRITE_BOOL(&edge->directed, fp);
      }
   }
}


// Print graph.
void Graph::print(FILE *fp)
{
   print(NULL, fp);
}


void Graph::print(char *label, FILE *fp)
{
   int i, i2, j, j2;

   if (label == NULL)
   {
      fprintf(fp, "Graph:\n");
   }
   else
   {
      fprintf(fp, "Graph: %s\n", label);
   }
   for (i = 0, i2 = (int)vertices.size(); i < i2; i++)
   {
      if (vertices[i]->label != NULL_LABEL)
      {
         fprintf(fp, "vertex %p, label = %d\n", vertices[i], vertices[i]->label);
      }
      else
      {
         fprintf(fp, "vertex %p\n", vertices[i]);
      }
      for (j = 0, j2 = (int)vertices[i]->edges.size(); j < j2; j++)
      {
         if (vertices[i]->edges[j]->directed)
         {
            if (vertices[i]->edges[j]->source == vertices[i])
            {
               if (vertices[i]->edges[j]->label != NULL_LABEL)
               {
                  fprintf(fp, "\t%p -> edge %p, label = %d -> %p\n", vertices[i]->edges[j]->source,
                          vertices[i]->edges[j], vertices[i]->edges[j]->label, vertices[i]->edges[j]->target);
               }
               else
               {
                  fprintf(fp, "\t%p -> edge %p -> %p\n", vertices[i]->edges[j]->source,
                          vertices[i]->edges[j], vertices[i]->edges[j]->target);
               }
            }
            else
            {
               if (vertices[i]->edges[j]->label != NULL_LABEL)
               {
                  fprintf(fp, "\t%p <- edge %p, label = %d <- %p\n", vertices[i]->edges[j]->target,
                          vertices[i]->edges[j], vertices[i]->edges[j]->label, vertices[i]->edges[j]->source);
               }
               else
               {
                  fprintf(fp, "\t%p <- edge %p <- %p\n", vertices[i]->edges[j]->target,
                          vertices[i]->edges[j], vertices[i]->edges[j]->source);
               }
            }
         }
         else
         {
            if (vertices[i]->edges[j]->label != NULL_LABEL)
            {
               fprintf(fp, "\t%p <- edge %p, label = %d -> %p\n", vertices[i]->edges[j]->source,
                       vertices[i]->edges[j], vertices[i]->edges[j]->label, vertices[i]->edges[j]->target);
            }
            else
            {
               fprintf(fp, "\t%p <- edge %p -> %p\n", vertices[i]->edges[j]->source,
                       vertices[i]->edges[j], vertices[i]->edges[j]->target);
            }
         }
      }
   }
}


// Dump graph in Graphviz "dot" format.
void Graph::dump(FILE *fp)
{
   dump(NULL, fp);
}


void Graph::dump(char *label, FILE *fp)
{
   fprintf(fp, "digraph {\n");
   if (label == NULL)
   {
      fprintf(fp, "\tgraph [size=\"8.5,11\",fontsize=14];\n");
   }
   else
   {
      fprintf(fp, "\tgraph [size=\"8.5,11\",fontsize=14,label=\"%s\"];\n", label);
   }
   dumpSub(fp);
   fprintf(fp, "}\n");
}


// Dump "guts" of graph.
void Graph::dumpSub(FILE *fp)
{
   int i, i2, j, j2;

   for (i = 0, i2 = (int)vertices.size(); i < i2; i++)
   {
      if (vertices[i]->label != NULL_LABEL)
      {
         fprintf(fp, "\t\"%p\" [label=%d,shape=ellipse];\n", vertices[i], vertices[i]->label);
      }
      else
      {
         fprintf(fp, "\t\"%p\" [label=\"\",shape=ellipse];\n", vertices[i]);
      }
   }
   for (i = 0, i2 = (int)vertices.size(); i < i2; i++)
   {
      for (j = 0, j2 = (int)vertices[i]->edges.size(); j < j2; j++)
      {
         if (vertices[i]->edges[j]->directed)
         {
            if (vertices[i]->edges[j]->source == vertices[i])
            {
               if (vertices[i]->edges[j]->label != NULL_LABEL)
               {
                  fprintf(fp, "\t\"%p\" -> \"%p\" [label=%d];\n",
                          vertices[i]->edges[j]->source, vertices[i]->edges[j]->target,
                          vertices[i]->edges[j]->label);
               }
               else
               {
                  fprintf(fp, "\t\"%p\" -> \"%p\";\n",
                          vertices[i]->edges[j]->source, vertices[i]->edges[j]->target);
               }
            }
         }
         else
         {
            if (vertices[i]->edges[j]->source == vertices[i])
            {
               if (vertices[i]->edges[j]->label != NULL_LABEL)
               {
                  fprintf(fp, "\t\"%p\" -> \"%p\" [label=%d,dir=both];\n",
                          vertices[i]->edges[j]->source, vertices[i]->edges[j]->target,
                          vertices[i]->edges[j]->label);
               }
               else
               {
                  fprintf(fp, "\t\"%p\" -> \"%p\" [dir=both];\n",
                          vertices[i]->edges[j]->source, vertices[i]->edges[j]->target);
               }
            }
         }
      }
   }
}


// Vertex constructor.
Graph::Vertex::Vertex(unsigned int label)
{
   id          = 0;
   this->label = label;
}


// List source edges.
void Graph::Vertex::sourceEdges(vector<Graph::Edge *>& edgeList)
{
   int i, i2;

   edgeList.clear();
   for (i = 0, i2 = (int)edges.size(); i < i2; i++)
   {
      if (edges[i]->source == this)
      {
         edgeList.push_back(edges[i]);
      }
   }
}


// Edge constructor.
Graph::Edge::Edge(unsigned int label)
{
   this->label = label;
   source      = target = NULL;
   directed    = true;
}
