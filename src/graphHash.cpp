// For conditions of distribution and use, see copyright notice in graphHash.hpp

/*
 * Compute an MD5 hash for a graph.
 */

#include "graphHash.hpp"

// Constructor.
#ifndef COLOR_REFINE
GraphHash::GraphHash(bool hashLabels)
{
   this->hashLabels = hashLabels;
   memset(code, 0, MD5_SIZE);
   graph          = NULL;
   vertexCoder    = NULL;
   partitionCount = 0;
}


#else
GraphHash::GraphHash()
{
   memset(code, 0, MD5_SIZE);
   graph          = NULL;
   vertexCoder    = NULL;
   partitionCount = 0;
}


#endif

// Destructor.
GraphHash::~GraphHash()
{
   if (vertexCoder != NULL)
   {
      delete vertexCoder;
   }
}


// Hash graph.
#ifndef COLOR_REFINE
#ifdef THREADS
void GraphHash::hash(Graph *graph, int numThreads, bool verbose)
#else
void GraphHash::hash(Graph *graph, bool verbose)
#endif
#else
void GraphHash::hash(Graph *graph)
#endif
{
   int             i, i2;
   VertexCoder     *child;
   VertexCoderLink *link;

   vector<Graph::Vertex *> vertexBranch;

   // Save graph.
   this->graph = graph;

   // Clear.
   memset(code, 0, MD5_SIZE);
   if (vertexCoder != NULL)
   {
      delete vertexCoder;
      vertexCoder = NULL;
   }
   partitionCount = 0;

   // Encode hash for graph using vertex coders.
   vertexCoder = new VertexCoder();
   assert(vertexCoder != NULL);
   for (i = 0, i2 = (int)graph->vertices.size(); i < i2; i++)
   {
      if (graph->vertices[i]->label != NO_HASH_VERTEX_LABEL)
      {
         child = new VertexCoder(graph->vertices[i], vertexBranch);
         assert(child != NULL);
         link = new VertexCoderLink(NULL, child);
         assert(link != NULL);
         vertexCoder->children.push_back(link);
      }
   }
#ifndef COLOR_REFINE
#ifdef THREADS
   vertexCoder->encode(numThreads, hashLabels, verbose);
#else
   vertexCoder->encode(hashLabels, verbose);
#endif
#else
   vertexCoder->encode();
#endif
   memcpy(code, vertexCoder->code, MD5_SIZE);

   // Count partitions.
   for (i = partitionCount = 0, i2 = (int)vertexCoder->children.size(); i < i2; i++)
   {
      if ((i == 0) || (memcmp(vertexCoder->children[i - 1]->coder->code,
                              vertexCoder->children[i]->coder->code, MD5_SIZE) != 0))
      {
         partitionCount++;
      }
   }
}


// Get graph hash.
// Valid after graph hash.
// Hash size is MD5_SIZE bytes (see md5.h).
unsigned char *GraphHash::getHash()
{
   if (vertexCoder != NULL)
   {
      return(vertexCoder->code);
   }
   else
   {
      return(NULL);
   }
}


// Print graph and its hash.
void GraphHash::print(FILE *fp)
{
   if (graph == NULL)
   {
      return;
   }
   graph->print(fp);
   fprintf(fp, "Hash: ");
   for (int i = 0; i < MD5_SIZE; i++)
   {
      fprintf(fp, "%d ", code[i]);
   }
   fprintf(fp, "\n");
}


// Less-than comparison of vertices by label.
bool GraphHash::ltcmpVertexLabels(Graph::Vertex *a, Graph::Vertex *b)
{
   return(a->label < b->label);
}


// Vertex coder link constructors.
GraphHash::VertexCoderLink::VertexCoderLink()
{
   edge  = NULL;
   coder = NULL;
}


GraphHash::VertexCoderLink::VertexCoderLink(Graph::Edge *edge, VertexCoder *coder)
{
   this->edge  = edge;
   this->coder = coder;
}


// Vertex coder constructors.
GraphHash::VertexCoder::VertexCoder()
{
   vertex = NULL;
   memset(code, 0, MD5_SIZE);
   codeValid  = false;
   labelClass = -1;
}


GraphHash::VertexCoder::VertexCoder(Graph::Vertex *vertex, vector<Graph::Vertex *> vertexBranch)
{
   this->vertex       = vertex;
   this->vertexBranch = vertexBranch;
   memset(code, 0, MD5_SIZE);
   codeValid  = false;
   labelClass = -1;
}


// Vertex coder destructor.
GraphHash::VertexCoder::~VertexCoder()
{
   int i, i2;

   for (i = 0, i2 = (int)children.size(); i < i2; i++)
   {
#ifdef COLOR_REFINE
      if (vertex == NULL)
#endif
      delete children[i]->coder;
      delete children[i];
   }
   children.clear();
}


// Threaded encode.
#ifndef COLOR_REFINE
#ifdef THREADS
void GraphHash::VertexCoder::encode(int numThreads, bool hashLabels, bool verbose)
{
   if (numThreads > 1)
   {
      int i, i2;
      vector<VertexCoder *> vertexList;
      for (i = 0, i2 = (int)children.size(); i < i2; i++)
      {
         vertexList.push_back(children[i]->coder);
      }

      // Start additional encoding threads.
      this->numThreads = numThreads;
      terminate        = false;
      int n      = 0;
      int result = pthread_barrier_init(&updateBarrier, NULL, numThreads);
      assert(result == 0);
      result = pthread_mutex_init(&updateMutex, NULL);
      assert(result == 0);
      threads = new pthread_t[numThreads - 1];
      assert(threads != NULL);
      struct ThreadInfo *info;
      for (int i = 0; i < numThreads - 1; i++)
      {
         info = new struct ThreadInfo;
         assert(info != NULL);
         info->coder      = this;
         info->threadNum  = i + 1;
         info->vertexList = &vertexList;
         info->hashLabels = hashLabels;
         info->verbose    = verbose;
         result           = pthread_create(&threads[i], NULL, encodeThread, (void *)info);
         assert(result == 0);
         n++;
      }

      // Encode vertices.
      encodeVertices(0, &vertexList, hashLabels, verbose);

      // Terminate threads.
      terminate = true;
      encodeVertices(0, &vertexList, hashLabels, verbose);
      for (int i = 0; i < n; i++)
      {
         pthread_join(threads[i], NULL);
         pthread_detach(threads[i]);
      }
      pthread_mutex_destroy(&updateMutex);
      pthread_barrier_destroy(&updateBarrier);
      delete threads;
   }

   // Encode graph.
   encode(hashLabels, verbose);
}


#endif
#endif

#ifndef COLOR_REFINE
// Recursively encode.
void GraphHash::VertexCoder::encode(bool hashLabels, bool verbose)
{
   int               i, i2, n, s;
   struct MD5Context *md5c;
   unsigned char     *input, *p;
   Graph::Edge       *edge;
   char              buf[20];

   if (codeValid)
   {
      return;
   }
   if (vertex != NULL)
   {
      expand();
   }
   s = (int)children.size();
   for (i = 0; i < s; i++)
   {
      if (verbose && (vertex == NULL) && !children[i]->coder->codeValid)
      {
         printf("Hash vertex %d/%d\n", i + 1, s);
      }
      children[i]->coder->encode(hashLabels, verbose);
      children[i]->coder->contract();
   }
   if (hashLabels)
   {
      sort(children.begin(), children.end(), ltcmpCoderLinksLabeled);
   }
   else
   {
      sort(children.begin(), children.end(), ltcmpCoderLinks);
   }
   n = 0;
   if (vertex != NULL)
   {
      if (hashLabels)
      {
         n += sizeof(vertex->label);
      }
      if (s > 0)
      {
         if (hashLabels)
         {
            for (i = 0; i < s; i++)
            {
               n += sizeof(children[i]->edge->label);
            }
         }
         n += s;
      }
      else
      {
         n += 8;
      }
   }
   input = new unsigned char[(s * MD5_SIZE) + n];
   assert(input != NULL);
   if (vertex != NULL)
   {
      p = input;
      if (hashLabels)
      {
         memcpy(input, &vertex->label, sizeof(vertex->label));
         p += sizeof(vertex->label);
      }
      if (s > 0)
      {
         for (i = 0; i < s; i++)
         {
            edge = children[i]->edge;
            if (hashLabels)
            {
               memcpy(p, &edge->label, sizeof(edge->label));
               p += sizeof(edge->label);
            }
            if (edge->directed)
            {
               if (edge->source == vertex)
               {
                  *p = 1;
               }
               else
               {
                  *p = 0;
               }
            }
            else
            {
               *p = 2;
            }
            p++;
         }
      }
      else
      {
         for (i = 0, i2 = (int)vertexBranch.size(); i < i2; i++)
         {
            if (vertex == vertexBranch[i])
            {
               break;
            }
         }
         i++;
         sprintf(buf, "%08d", i);
         memcpy(p, buf, 8);
      }
   }
   for (i = 0; i < s; i++)
   {
      memcpy(&input[n], children[i]->coder->code, MD5_SIZE);
      n += MD5_SIZE;
   }
   md5c = new struct MD5Context;
   assert(md5c != NULL);
   MD5Init(md5c);
   MD5Update(md5c, input, n);
   MD5Final(code, md5c);
   codeValid = true;
   delete md5c;
   delete[] input;
   if (verbose && (vertex == NULL))
   {
      printf("Done\n");
   }
}


#else
void GraphHash::VertexCoder::encode()
{
   int               i, j, i2, j2, k, n, s;
   Graph::Vertex     *v1, *v2;
   VertexCoder       *coder;
   VertexCoderLink   *link;
   struct MD5Context *md5c;
   unsigned char     *input;

   s = (int)children.size();
   for (i = 0; i < s; i++)
   {
      coder = children[i]->coder;
      memset(coder->code, 0, MD5_SIZE);
      v1 = coder->vertex;
      for (j = 0, j2 = (int)v1->edges.size(); j < j2; j++)
      {
         if (v1 == v1->edges[j]->source)
         {
            v2 = v1->edges[j]->target;
         }
         else
         {
            v2 = v1->edges[j]->source;
         }
         for (k = 0; k < s; k++)
         {
            if (children[k]->coder->vertex == v2)
            {
               link = new VertexCoderLink(v1->edges[j], children[k]->coder);
               assert(link != NULL);
               coder->children.push_back(link);
               break;
            }
         }
      }
   }

   for (i = 0; i < s; i++)
   {
      for (i2 = 0; i2 < s; i2++)
      {
         coder = children[i2]->coder;
         sort(coder->children.begin(), coder->children.end(), ltcmpCoderLinks);
         n     = (int)coder->children.size();
         input = new unsigned char[n * MD5_SIZE];
         assert(input != NULL);
         for (j = k = 0; j < n; j++)
         {
            memcpy(&input[k], coder->children[j]->coder->code, MD5_SIZE);
            k += MD5_SIZE;
         }
         md5c = new struct MD5Context;
         assert(md5c != NULL);
         MD5Init(md5c);
         MD5Update(md5c, input, k);
         MD5Final(coder->newcode, md5c);
         delete md5c;
         delete[] input;
      }

      for (i2 = 0; i2 < s; i2++)
      {
         coder = children[i2]->coder;
         memcpy(coder->code, coder->newcode, MD5_SIZE);
         coder->codeValid = true;
      }
   }

   sort(children.begin(), children.end(), ltcmpCoderLinks);
   input = new unsigned char[s * MD5_SIZE];
   assert(input != NULL);
   for (i = j = 0; i < s; i++)
   {
      memcpy(&input[j], children[i]->coder->code, MD5_SIZE);
      j += MD5_SIZE;
   }
   md5c = new struct MD5Context;
   assert(md5c != NULL);
   MD5Init(md5c);
   MD5Update(md5c, input, j);
   MD5Final(code, md5c);
   codeValid = true;
   delete md5c;
   delete[] input;
}


#endif

#ifndef COLOR_REFINE
// Expand coder.
void GraphHash::VertexCoder::expand()
{
   int i, i2;

   vector<Graph::Vertex *> childVertexBranch;
   Graph::Vertex           *childVertex;
   VertexCoder             *child;
   VertexCoderLink         *link;

   for (i = 0, i2 = (int)vertexBranch.size(); i < i2; i++)
   {
      if (vertex == vertexBranch[i])
      {
         return;
      }
      childVertexBranch.push_back(vertexBranch[i]);
   }
   childVertexBranch.push_back(vertex);
   for (i = 0, i2 = (int)vertex->edges.size(); i < i2; i++)
   {
      if (vertex == vertex->edges[i]->source)
      {
         childVertex = vertex->edges[i]->target;
      }
      else
      {
         childVertex = vertex->edges[i]->source;
      }
      if (childVertex->label != NO_HASH_VERTEX_LABEL)
      {
         child = new VertexCoder(childVertex, childVertexBranch);
         assert(child != NULL);
         link = new VertexCoderLink(vertex->edges[i], child);
         assert(link != NULL);
         children.push_back(link);
      }
   }
}


// Contract coder.
void GraphHash::VertexCoder::contract()
{
   int i, i2;

   for (i = 0, i2 = (int)children.size(); i < i2; i++)
   {
      delete children[i]->coder;
      delete children[i];
   }
   children.clear();
}


// Encode vertices.
#ifdef THREADS
void GraphHash::VertexCoder::encodeVertices(int threadNum, vector<VertexCoder *> *vertexList,
                                            bool hashLabels, bool verbose)
{
   // Synchronize threads.
   int s = vertexList->size();
   int i = pthread_barrier_wait(&updateBarrier);

   if ((i != PTHREAD_BARRIER_SERIAL_THREAD) && (i != 0))
   {
      pthread_exit(NULL);
   }
   if (terminate)
   {
      if (threadNum == 0)
      {
         return;
      }
      pthread_exit(NULL);
   }

   // Update vertices.
   int n;
   while (true)
   {
      pthread_mutex_lock(&updateMutex);
      VertexCoder *v = NULL;
      if (!vertexList->empty())
      {
         v = vertexList->back();
         vertexList->pop_back();
         n = s - vertexList->size();
      }
      pthread_mutex_unlock(&updateMutex);
      if (v == NULL)
      {
         break;
      }
      if (verbose)
      {
         printf("Hash vertex %d/%d, thread=%d\n", n, s, threadNum);
      }
      v->encode(hashLabels, verbose);
   }

   // Re-group threads.
   pthread_barrier_wait(&updateBarrier);
}


// Vertex expand thread.
void *GraphHash::VertexCoder::encodeThread(void *arg)
{
   struct ThreadInfo      *info     = (struct ThreadInfo *)arg;
   int                    threadNum = info->threadNum;
   GraphHash::VertexCoder *coder    = info->coder;

   vector<VertexCoder *> *vertexList = info->vertexList;
   bool hashLabels = info->hashLabels;
   bool verbose    = info->verbose;
   delete info;
   while (true)
   {
      coder->encodeVertices(threadNum, vertexList, hashLabels, verbose);
   }
   return(NULL);
}


#endif
#endif

// Print code.
void GraphHash::VertexCoder::printCode(FILE *fp)
{
   for (int i = 0; i < MD5_SIZE; i++)
   {
      fprintf(fp, "%d ", code[i]);
   }
   fprintf(fp, "\n");
}


// Less-than comparison of coder links.
bool GraphHash::VertexCoder::ltcmpCoderLinks(VertexCoderLink *a, VertexCoderLink *b)
{
   if (cmpCoderLinks(a, b) < 0)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// Less-than comparison of labeled coder links.
bool GraphHash::VertexCoder::ltcmpCoderLinksLabeled(VertexCoderLink *a, VertexCoderLink *b)
{
   int result = cmpCoderLinks(a, b);

   if (result < 0)
   {
      return(true);
   }
   else if (result == 0)
   {
      if (a->edge == NULL)
      {
         return(false);
      }
      else
      {
         if (a->edge->label < b->edge->label)
         {
            return(true);
         }
         else
         {
            return(false);
         }
      }
   }
   else
   {
      return(false);
   }
}


// Comparison of coder links.
int GraphHash::VertexCoder::cmpCoderLinks(VertexCoderLink *a, VertexCoderLink *b)
{
   int result = memcmp(a->coder->code, b->coder->code, MD5_SIZE);

   if (result < 0)
   {
      return(-1);
   }
   else if (result == 0)
   {
      if (a->edge == NULL)
      {
         return(0);
      }
      else
      {
         if (a->edge->directed)
         {
            if (b->edge->directed)
            {
               if (a->edge->source == a->coder->vertex)
               {
                  if (b->edge->source == b->coder->vertex)
                  {
                     return(0);
                  }
                  else
                  {
                     return(-1);
                  }
               }
               else
               {
                  if (b->edge->source == b->coder->vertex)
                  {
                     return(1);
                  }
                  else
                  {
                     return(0);
                  }
               }
            }
            else
            {
               return(-1);
            }
         }
         else
         {
            if (b->edge->directed)
            {
               return(1);
            }
            else
            {
               return(0);
            }
         }
      }
   }
   else
   {
      return(1);
   }
}
