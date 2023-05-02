#include "graph_ge.h"

/*** record for qsorting a graph to provide another isomorphic to it ***/
#define RecordSize    24
typedef struct
{
   char record[RecordSize];
} Record;

int zero_vertex_or_edge_count(void);
int unpicked_vertices_p(int *closed, int n);
int pick_to(int *closed, int vertices);
void r_graph(int v,
             int e,
             int max_wgt, int weight_flag,
             Graph **graph);
void rename_edges(Record *ptr, int *aliases, int count);
void rename_vertices(int *aliases, int count);
int compare_strings(const void *a, const void *b);
void warning(char *message);
int ran(int k);              /* customized random number generator */
void permute(int *a, int n); /* gives a random permutation */

/* initialize array a so that a[ i ] = i */
void init_array(int *a, int end);

void swap(int *a, int *b);   /* swap two ints */

void create_graph(
   int   v,
   int   e,
   Graph **out_graph,
   int   *adj_matrix,
   int   dir_flag);

/*** miscellany ***/
#define True                     1
#define False                    0
#define None                     -999
#define Undirected               0
#define Directed                 1

/*** graph generators ***/
#define RandomGraph              1
#define RandomDigraph            2
#define RandomConnectedGraph     3
#define DirectedAcyclicGraph     4
#define CompleteGraph            5
#define IsomorphicGraphPair      6
#define HamiltonianCycleGraph    7
#define NetworkFlowGraph         8

/*** warnings ***/
#define IllegalDimensions        "\n\n\tNeither edge nor vertex count may be zero.\n\n"
#define InsufficientStorage      "\n\n\tThere is not enough space for this size graph.\n\n"
#define SingleVertexNetwork      "\n\n\tA single vertex network is not allowed.\n\n"

/* min, max, odd */
#undef min
#define min(x, y)    (((x) < (y)) ? (x) : (y))

#undef max
#define max(x, y)    (((x) > (y)) ? (x) : (y))

#define odd(num)     ((num) % 2)

void warning(char *message)
{
   printf(message);
}


/******************** graph generators **************************/

/* This function writes a pair of isomorphic, regular,
 * undirected graphs to two files and writes an isomorphism to a third
 * file.
 *
 * To  generate  a  pair  of isomorphic simple and regular
 * graphs,  we  first  generate  a  random  simple  and regular
 * graph.  We then construct an isomorphic copy by renaming its
 * vertices using a random permutation.
 *
 * To generate a random simple graph
 * in   which  every  vertex  has  degree  r, we  execute  the
 * following,  which  we  subsequently  refer  to  as  the main
 * procedure:
 *
 *   1.   Choose  vertices  v and w each of degree less than
 *        r  such  that there is no edge between v and w. If
 *        no such pair exists, execute the fixup procedure.
 *
 *   2.   Add an edge between v and w.
 *
 *   3.   If some vertex has degree less than r, go to 1.
 *
 * The  fixup  procedure  works as follows. There are two cases
 * to consider:
 *
 *   1.   All vertices except one have degree equal to r.
 *
 *   2.   There  are  pairs  of vertices each of degree less
 *        than  r,  but  each  such  pair is connected by an
 *        edge.
 *
 * In  the  first case, we increase the edge count in the graph
 * in  the following way. Suppose that vertex v has degree less
 * than  r.  Choose an edge (w1,w2) such that (v,w1) and (v,w2)
 * are  not edges. Delete the edge (w1,w2) and add edges (v,w1)
 * and  (v,w2).  In the second case, we increase the edge count
 * in  the graph in a similar way. Suppose that vertices v1 and
 * v2  have  degree  less than r and (v1,v2) is an edge. Choose
 * an  edge  (w1,w2)  such  that  (v1,w1)  and  (v2,w2) are not
 * edges.  Delete  the  edge  (w1,w2) and add edges (v1,w1) and
 * (v2,w2).  If  some  vertex in the resulting graph has degree
 * less than r, we resume by executing the main procedure.
 */
void isomorphic_graph_pair(int   vertices,
                           int   spec_degree,
                           Graph **first,
                           Graph **second)
{
   Record *records, *ptr;
   int    i, j,
          size       = vertices * vertices,
          edge_count = 0,
          edges      = (vertices * spec_degree) / 2,
          base, offset, index1, index2, index3,
          v, v1, v2,
          w, w1, w2,
   *adj_matrix,
   *vertex_degree,
   *aux,
   *aliases;

   /* adjacency matrix to represent graph */
   if ((adj_matrix = ( int * )calloc(size, sizeof(int))) == NULL)
   {
      warning(InsufficientStorage);
      return;
   }

   /* vectors to hold degree count per vertex */
   if ((vertex_degree = ( int * )calloc(vertices, sizeof(int))) == NULL)
   {
      warning(InsufficientStorage);
      free(adj_matrix);
      return;
   }
   if ((aux = ( int * )calloc(vertices, sizeof(int))) == NULL)
   {
      warning(InsufficientStorage);
      free(adj_matrix);
      free(vertex_degree);
      return;
   }

   /* vector to hold renamed vertices */
   if ((aliases = ( int * )calloc(vertices, sizeof(int))) == NULL)
   {
      warning(InsufficientStorage);
      free(adj_matrix);
      free(vertex_degree);
      free(aux);
      return;
   }

   /* file entries to be sorted to get isomorphic graph */
   if ((records = ( Record * )calloc(edges, sizeof(Record))) == NULL)
   {
      warning(InsufficientStorage);
      free(adj_matrix);
      free(vertex_degree);
      free(aux);
      free(aliases);
      return;
   }
   else
   {
      ptr = records;
   }

   /* Initialize adjacency matrix for random permutation. */
   init_array(aux, vertices);

   /* loop until every vertex has spec_degree */
   while (edge_count < edges)
   {
      permute(aux, vertices);
      w = -1;

      /* Look for 2 vertices whose degree is less than r.
       * If found & no edge, put in an edge & repeat.
       * If we can find 2 vertices whose degree
       * is less than r, but each such pair is joined by an edge,
       * set w != -1 and record one such pair as v1, w1.
       * If there is no pair of vertices whose degree is less than r,
       * w will still be -1 and there will be
       * exactly one vertex v whose degree is less than r.  */
      for (i = 0; i < vertices; i++)
      {
         if (vertex_degree[aux[i]] < spec_degree)
         {
            v = aux[i];
            for (j = i + 1; j < vertices; j++)
            {
               if (vertex_degree[aux[j]] < spec_degree)
               {
                  w      = aux[j];
                  base   = min(v, w);
                  offset = max(v, w);
                  index1 = (base * vertices) + offset;
                  if (!adj_matrix[index1])       /* no edge so add one */
                  {
                     adj_matrix[index1] = 1;
                     vertex_degree[v]++;
                     vertex_degree[w]++;
                     goto next;
                  }
                  else   /* save the vertices where there is an edge */
                  {
                     v1 = v;
                     w1 = w;
                  }
               }
            }
         }
      }
      if (w == -1)     /* only one vertex v with degree < spec_degree */
      {
         v1 = v;

         /* Find an edge (v2,w2) such that
          *
          *    v2 != v1 && w2 != v1 && there are no edges (v1,v2)
          *    and (v1,w2).
          *
          * Add edges (v1,v2) and (v1,w2) and delete edge (v2,w2).  */
         for (i = 0; i < vertices - 1; i++)
         {
            if (v1 != (v2 = aux[i]))
            {
               base   = min(v1, v2);
               offset = max(v1, v2);
               index1 = (base * vertices) + offset;
               if (!adj_matrix[index1])
               {
                  for (j = i + 1; j < vertices; j++)
                  {
                     if ((w2 = aux[j]) != v1)
                     {
                        base   = min(v2, w2);
                        offset = max(v2, w2);
                        index2 = (base * vertices) + offset;
                        base   = min(v1, w2);
                        offset = max(v1, w2);
                        index3 = (base * vertices) + offset;
                        if (adj_matrix[index2] && !adj_matrix[index3])
                        {
                           adj_matrix[index1]    =
                              adj_matrix[index3] = 1;
                           adj_matrix[index2]    = 0;
                           vertex_degree[v1]++;
                           vertex_degree[v1]++;
                           goto next;
                        }
                     }
                  }
               }
            }
         }
      }
      else    /* two vertices v1 and w1 with degree < spec_degree */
      {
         /* Find an edge (v2,w2) such that
          *
          *    v2 != v1 && w1 != v2 && w2 != v1 && w2 != w1
          *    and there are no edges (v1,v2) and (w1,w2).
          *
          * Add edges (v1,v2) and (w1,w2) and delete edge (v2,w2).
          *
          * [Note that both loops must run from 0 to n-1 since
          * there is an asymmetry in the conditions. For example,
          * there might be an edge (v2,w2) such that there is an
          * edge (v1,v2) yet there are no edges (v1,w2) and (w1,v2).]      */
         for (i = 0; i < vertices; i++)
         {
            if ((v1 != (v2 = aux[i])) && (v2 != w1))
            {
               base   = min(v1, v2);
               offset = max(v1, v2);
               index1 = (base * vertices) + offset;
               if (!adj_matrix[index1])
               {
                  for (j = 0; j < vertices; j++)
                  {
                     if (((w2 = aux[j]) != v1) && (w2 != w1))
                     {
                        base   = min(v2, w2);
                        offset = max(v2, w2);
                        index2 = (base * vertices) + offset;
                        base   = min(w1, w2);
                        offset = max(w1, w2);
                        index3 = (base * vertices) + offset;
                        if (adj_matrix[index2] && !adj_matrix[index3])
                        {
                           adj_matrix[index2]    = 0;
                           adj_matrix[index1]    =
                              adj_matrix[index3] = 1;
                           vertex_degree[v1]++;
                           vertex_degree[w1]++;
                           goto next;
                        }
                     }
                  }
               }
            }
         }
      }
next: edge_count++;
   }

   /* create first graph */
   create_graph(vertices, edges, first, adj_matrix, Undirected);

   /* create an isomorphism */
   rename_vertices(aliases, vertices);
   create_graph(vertices, edges, second, adj_matrix, Undirected);

   free(adj_matrix);
   free(vertex_degree);
   free(aux);
   free(aliases);
   free(records);
}


// Create regular graph.
void regular_graph(int   vertices,
                   int   spec_degree,
                   Graph **out_graph)
{
   Record *records, *ptr;
   int    i, j,
          size       = vertices * vertices,
          edge_count = 0,
          edges      = (vertices * spec_degree) / 2,
          base, offset, index1, index2, index3,
          v, v1, v2,
          w, w1, w2,
   *adj_matrix,
   *vertex_degree,
   *aux,
   *aliases;

   /* adjacency matrix to represent graph */
   if ((adj_matrix = (int *)calloc(size, sizeof(int))) == NULL)
   {
      warning(InsufficientStorage);
      return;
   }

   /* vectors to hold degree count per vertex */
   if ((vertex_degree = (int *)calloc(vertices, sizeof(int))) == NULL)
   {
      warning(InsufficientStorage);
      free(adj_matrix);
      return;
   }
   if ((aux = (int *)calloc(vertices, sizeof(int))) == NULL)
   {
      warning(InsufficientStorage);
      free(adj_matrix);
      free(vertex_degree);
      return;
   }

   /* vector to hold renamed vertices */
   if ((aliases = (int *)calloc(vertices, sizeof(int))) == NULL)
   {
      warning(InsufficientStorage);
      free(adj_matrix);
      free(vertex_degree);
      free(aux);
      return;
   }

   /* file entries to be sorted to get isomorphic graph */
   if ((records = (Record *)calloc(edges, sizeof(Record))) == NULL)
   {
      warning(InsufficientStorage);
      free(adj_matrix);
      free(vertex_degree);
      free(aux);
      free(aliases);
      return;
   }
   else
   {
      ptr = records;
   }

   /* Initialize adjacency matrix for random permutation. */
   init_array(aux, vertices);

   /* loop until every vertex has spec_degree */
   while (edge_count < edges)
   {
      permute(aux, vertices);
      w = -1;

      /* Look for 2 vertices whose degree is less than r.
       * If found & no edge, put in an edge & repeat.
       * If we can find 2 vertices whose degree
       * is less than r, but each such pair is joined by an edge,
       * set w != -1 and record one such pair as v1, w1.
       * If there is no pair of vertices whose degree is less than r,
       * w will still be -1 and there will be
       * exactly one vertex v whose degree is less than r.  */
      for (i = 0; i < vertices; i++)
      {
         if (vertex_degree[aux[i]] < spec_degree)
         {
            v = aux[i];
            for (j = i + 1; j < vertices; j++)
            {
               if (vertex_degree[aux[j]] < spec_degree)
               {
                  w      = aux[j];
                  base   = min(v, w);
                  offset = max(v, w);
                  index1 = (base * vertices) + offset;
                  if (!adj_matrix[index1])                                 /* no edge so add one */
                  {
                     adj_matrix[index1] = 1;
                     vertex_degree[v]++;
                     vertex_degree[w]++;
                     goto next;
                  }
                  else                                 /* save the vertices where there is an edge */
                  {
                     v1 = v;
                     w1 = w;
                  }
               }
            }
         }
      }
      if (w == -1)             /* only one vertex v with degree < spec_degree */
      {
         v1 = v;

         /* Find an edge (v2,w2) such that
          *
          * v2 != v1 && w2 != v1 && there are no edges (v1,v2)
          * and (v1,w2).
          *
          * Add edges (v1,v2) and (v1,w2) and delete edge (v2,w2).  */
         for (i = 0; i < vertices - 1; i++)
         {
            if (v1 != (v2 = aux[i]))
            {
               base   = min(v1, v2);
               offset = max(v1, v2);
               index1 = (base * vertices) + offset;
               if (!adj_matrix[index1])
               {
                  for (j = i + 1; j < vertices; j++)
                  {
                     if ((w2 = aux[j]) != v1)
                     {
                        base   = min(v2, w2);
                        offset = max(v2, w2);
                        index2 = (base * vertices) + offset;
                        base   = min(v1, w2);
                        offset = max(v1, w2);
                        index3 = (base * vertices) + offset;
                        if (adj_matrix[index2] && !adj_matrix[index3])
                        {
                           adj_matrix[index1]    =
                              adj_matrix[index3] = 1;
                           adj_matrix[index2]    = 0;
                           vertex_degree[v1]++;
                           vertex_degree[v1]++;
                           goto next;
                        }
                     }
                  }
               }
            }
         }
      }
      else              /* two vertices v1 and w1 with degree < spec_degree */
      {
         /* Find an edge (v2,w2) such that
          *
          * v2 != v1 && w1 != v2 && w2 != v1 && w2 != w1
          * and there are no edges (v1,v2) and (w1,w2).
          *
          * Add edges (v1,v2) and (w1,w2) and delete edge (v2,w2).
          *
          * [Note that both loops must run from 0 to n-1 since
          * there is an asymmetry in the conditions. For example,
          * there might be an edge (v2,w2) such that there is an
          * edge (v1,v2) yet there are no edges (v1,w2) and (w1,v2).]      */
         for (i = 0; i < vertices; i++)
         {
            if ((v1 != (v2 = aux[i])) && (v2 != w1))
            {
               base   = min(v1, v2);
               offset = max(v1, v2);
               index1 = (base * vertices) + offset;
               if (!adj_matrix[index1])
               {
                  for (j = 0; j < vertices; j++)
                  {
                     if (((w2 = aux[j]) != v1) && (w2 != w1))
                     {
                        base   = min(v2, w2);
                        offset = max(v2, w2);
                        index2 = (base * vertices) + offset;
                        base   = min(w1, w2);
                        offset = max(w1, w2);
                        index3 = (base * vertices) + offset;
                        if (adj_matrix[index2] && !adj_matrix[index3])
                        {
                           adj_matrix[index2]    = 0;
                           adj_matrix[index1]    =
                              adj_matrix[index3] = 1;
                           vertex_degree[v1]++;
                           vertex_degree[w1]++;
                           goto next;
                        }
                     }
                  }
               }
            }
         }
      }
next: edge_count++;
   }

   /* create graph */
   create_graph(vertices, edges, out_graph, adj_matrix, Undirected);

   free(adj_matrix);
   free(vertex_degree);
   free(aux);
   free(aliases);
   free(records);
}


/* This function serves the same purpose as strcmp.  However,
 * the arguments in compare_strings are of type const void*, thus
 * providing compatibility with the ANSI function prototype header for qsort.
 * If strcmp is substituted for compare_strings in the qsort
 * invocation, in ANSI C a warning will be issued.
 * (Notice that in ANSI C, the const void* types are automatically converted to
 * const char* when strcmp is called.)
 */
int compare_strings(const void *a, const void *b)
{
   return(strcmp((char *)a, (char *)b));
}


void rename_vertices(int *aliases, int count)
{
   init_array(aliases, count);
   permute(aliases, count);
}


void rename_edges(Record *ptr, int *aliases, int count)
{
   int  i, vertex1, vertex2, v1, v2;
   char buffer[RecordSize];

   for (i = 0; i < count; i++)
   {
      sscanf(ptr->record, "%5d  %5d",
             &vertex1, &vertex2);
      if ((v1 = aliases[vertex1 - 1] + 1) <
          (v2 = aliases[vertex2 - 1] + 1))
      {
         sprintf(buffer, "%5d   %5d   %5d", v1, v2, 1);
      }
      else
      {
         sprintf(buffer, "%5d   %5d   %5d", v2, v1, 1);
      }
      strcpy(ptr->record, buffer);
      ptr++;
   }
}


/* This function generates a random connected simple graph with
 * v vertices and max(v-1,e) edges.  The graph can be weighted
 * (weight_flag == 1) or unweighted (weight_flag != 1). If
 * it is weighted, the weights are in the range 1 to max_wgt.
 * It is assumed that e <= v(v-1)/2. (In this program, this assured
 * because of the call to fix_imbalanced_graph.)
 *
 * To  generate  a  random  connected  graph,  we begin by
 * generating  a  random  spanning  tree.  To generate a random
 * spanning  tree,  we  first  generate  a  random  permutation
 * tree[0],...,tree[v-1]. (v = number of vertices.)
 * We  then  iteratively  add edges  to form a
 * tree.  We  begin with the tree consisting of vertex tree[0] and
 * no   edges.   At   the   iterative   step,  we  assume  that
 * tree[0],tree[1],...,tree[i-1]  are  in  the  tree.  We  then add vertex
 * tree[i]  to     the    tree    by    adding    the    edge
 * (tree[i],tree[rand(i)]).  (This  construction  is similar to
 * that  of  Prim's algorithm.) Finally, we add random edges to
 * produce the desired number of edges.
 */
void random_connected_graph(int   v,
                            int   e,
                            int   max_wgt,
                            int   weight_flag,
                            Graph **out_graph)
{
   int i, j, count, index, *adj_matrix, *tree;

   if ((adj_matrix = ( int * )calloc(v * v, sizeof(int)))
       == NULL)
   {
      printf("Not enough room for this size graph\n");
      return;
   }

   if ((tree = ( int * )calloc(v, sizeof(int))) == NULL)
   {
      printf("Not enough room for this size graph\n");
      free(adj_matrix);
      return;
   }

   printf("\n\tBeginning construction of graph.\n");

   /*  Generate a random permutation in the array tree. */
   init_array(tree, v);
   permute(tree, v);

   /*  Next generate a random spanning tree.
    *  The algorithm is:
    *
    *    Assume that vertices tree[ 0 ],...,tree[ i - 1 ] are in
    *    the tree.  Add an edge incident on tree[ i ]
    *    and a random vertex in the set {tree[ 0 ],...,tree[ i - 1 ]}.
    */

   for (i = 1; i < v; i++)
   {
      j = ran(i);
      adj_matrix[tree[i] * v + tree[j]]    =
         adj_matrix[tree[j] * v + tree[i]] =
            weight_flag ? 1 + ran(max_wgt) : 1;
   }

   /* Add additional random edges until achieving at least desired number */

   for (count = v - 1; count < e; )
   {
      i = ran(v);
      j = ran(v);

      if (i == j)
      {
         continue;
      }

      if (i > j)
      {
         swap(&i, &j);
      }

      index = i * v + j;
      if (!adj_matrix[index])
      {
         adj_matrix[index] = weight_flag ? 1 + ran(max_wgt) : 1;
         count++;
      }
   }

   //print_graph( v, count, out_file, adj_matrix, Undirected );

   free(tree);
   free(adj_matrix);
}


/* This function generates a random undirected graph with
 * v vertices and e edges.  The graph can be weighted
 * (weight_flag == 1) or unweighted (weight_flag != 1). If
 * it is weighted, the weights are in the range 1 to max_wgt.
 * The graph can be simple (simple_flag == 1) or not simple
 * (simple_flag != 1). If the graph is simple, it is assumed
 * that e <= v(v-1)/2. (In this program, this assured because
 * of the call to fix_imbalanced_graph.)
 */
void random_graph(int   v,
                  int   e,
                  int   max_wgt,
                  int   weight_flag,
                  int   simple_flag,
                  Graph **out_graph)
{
   int i, j, count, index, *adj_matrix;

   /* generate a not necessarily simple random graph */
   if (!simple_flag)
   {
      r_graph(v, e, max_wgt, weight_flag, out_graph);
      return;
   }

   /* generate a simple random graph */
   if ((adj_matrix = ( int * )calloc(v * v, sizeof(int)))
       == NULL)
   {
      printf("Not enough room for this size graph\n");
      return;
   }

   for (count = 0; count < e; )
   {
      i = ran(v);
      j = ran(v);

      if (i == j)
      {
         continue;
      }

      if (i > j)
      {
         swap(&i, &j);
      }

      index = i * v + j;
      if (!adj_matrix[index])
      {
         adj_matrix[index] = weight_flag ? 1 + ran(max_wgt) : 1;
         count++;
      }
   }

   //print_graph( v, e, out_file, adj_matrix, Undirected );

   free(adj_matrix);
}


/* This function generates a random, not necessarily simple
 * graph. The graph can be weighted (weight_flag == 1)
 * or unweighted (weight_flag != 1). It can be interpreted
 * as directed or undirected.
 *
 * Random edges are generated and written directly to the
 * output file.
 */
void r_graph(int v,
             int e,
             int max_wgt, int weight_flag,
             Graph **out_graph)
{
   //FILE *fp;
   //int i;

   //if ( ( fp = fopen( out_file, "w" ) ) == NULL ) {
   //   printf( "Unable to open file %s for writing\n", out_file );
   //   return;
   //}

   //printf( "\n\tWriting graph to file %s.\n", out_file );

   //fprintf( fp, "%5d   %5d\n", v, e );

   //for ( i = 0; i < e; i++ )
   //   fprintf( fp, "%5d   %5d   %5d\n", 1 + ran( v ), 1 + ran( v ),
   //            weight_flag ? 1 + ran( max_wgt ) : 1 );

   //printf( "\tGraph is written to file %s.\n", out_file );
   //fclose( fp );
}


/* This function generates a random directed graph with
 * v vertices and e edges.  The graph can be weighted
 * (weight_flag == 1) or unweighted (weight_flag != 1). If
 * it is weighted, the weights are in the range 1 to max_wgt.
 * The graph can be simple (simple_flag == 1) or not simple
 * (simple_flag != 1). If the graph is simple, it is assumed
 * that e <= v(v-1). (In this program, this assured because
 * of the call to fix_imbalanced_graph.)
 */
void random_digraph(int   v,
                    int   e,
                    int   max_wgt,
                    int   weight_flag,
                    int   simple_flag,
                    Graph **out_graph)
{
   int i, j, count, index, *adj_matrix;

   /* generate a not necessarily simple random digraph */
   if (!simple_flag)
   {
      r_graph(v, e, max_wgt, weight_flag, out_graph);
      return;
   }

   /* generate a simple random digraph */
   if ((adj_matrix = ( int * )calloc(v * v, sizeof(int)))
       == NULL)
   {
      printf("Not enough room for this size graph\n");
      return;
   }

   for (count = 0; count < e; )
   {
      i = ran(v);
      j = ran(v);

      if (i == j)
      {
         continue;
      }

      index = i * v + j;
      if (!adj_matrix[index])
      {
         adj_matrix[index] = weight_flag ? 1 + ran(max_wgt) : 1;
         count++;
      }
   }

   //print_graph( v, e, out_file, adj_matrix, Directed );
   free(adj_matrix);
}


/* This function writes a directed acyclic graph to one file and a
 * topological sort to a second file. The graph can be weighted or
 * unweighted. It is assumed that e <= v(v-1)/2. (In this program,
 * this assured because of the call to fix_imbalanced_graph.)
 *
 * To   generate   a  directed  acyclic  graph,  we  first
 * generate  a  random  permutation  dag[0],...,dag[v-1].
 * (v = number of vertices.)
 * This random permutation serves as a topological
 * sort  of  the  graph. We then generate random edges of the
 * form (dag[i],dag[j]) with i < j.
 */
void directed_acyclic_graph(int   v,
                            int   e,
                            int   max_wgt,
                            int   weight_flag,
                            Graph **out_graph)
{
   int i, j, count, index, *adj_matrix, *dag;

   if ((adj_matrix = ( int * )calloc(v * v, sizeof(int)))
       == NULL)
   {
      printf("Not enough room for this size graph\n");
      return;
   }

   if ((dag = ( int * )calloc(v, sizeof(int))) == NULL)
   {
      printf("Not enough room for this size graph\n");
      free(adj_matrix);
      return;
   }

   printf("\n\tBeginning construction of graph.\n");

   /*  Generate a random permutation in the array dag. */
   init_array(dag, v);
   permute(dag, v);

   for (count = 0; count < e; )
   {
      if ((i = ran(v)) == (j = ran(v)))
      {
         continue;
      }
      if (i > j)
      {
         swap(&i, &j);
      }
      i     = dag[i];
      j     = dag[j];
      index = i * v + j;
      if (!adj_matrix[index])
      {
         adj_matrix[index] = weight_flag ? 1 + ran(max_wgt) : 1;
         count++;
      }
   }

   //print_graph( v, e, out_file, adj_matrix, Directed );

   free(adj_matrix);
   free(dag);
}


/* This function generates a weighted complete graph.  The
 * graph can be directed or not.
 */
void complete_graph(int   v,
                    int   max_wgt,
                    int   dir_flag,
                    Graph **out_graph)
{
   //int i, j;
   //FILE *fp;

   //if ( ( fp = fopen( out_file, "w" ) ) == NULL ) {
   //   printf( "Unable to open file %s for writing.\n", out_file );
   //   return;
   //}
   //printf( "\n\tWriting graph to file %s.\n", out_file );

   //fprintf( fp, "%5d   %5d\n", v,
   //         dir_flag ? v * ( v - 1 ) : v * ( v - 1 ) / 2 );

   //for ( i = 1; i < v; i++ )
   //   for ( j = i + 1; j <= v; j++ ) {
   //      fprintf( fp, "%5d   %5d   %5d\n", i, j, 1 + ran( max_wgt ) );
   //      if ( dir_flag )
   //         fprintf( fp, "%5d   %5d   %5d\n", j, i, 1 + ran( max_wgt ) );
   //   }

   //fclose( fp );

   //printf( "\tGraph is written to file %s.\n", out_file );
}


/* This function writes a simple graph with a Hamiltonian cycle to one
 * file and a Hamiltonian cycle to another file. The graph will
 * have max(e,v) edges. The graph can be directed or undirected. It is
 * assumed that e <= v(v-1)/2 if the graph is undirected, and that
 * e <= v(v-1) if the grpah is directed. (In this program,
 * this assured because of the call to fix_imbalanced_graph.)
 *
 * To generate a random graph with a
 * Hamiltonian cycle, we begin with a random permutation
 * ham[0],...,ham[v-1] (v = number of vertices).  We then generate edges
 *
 * (ham[0],ham[1]),(ham[1],ham[2]),...,(ham[v-2],ham[v-1]),(ham[v-1],ham[0]),
 *
 * so that the graph has the Hamiltonian cycle
 *
 *                   ham[0],...,ham[v-1],ham[0].
 *
 * Finally,  we  add random edges to produce the desired number
 * of edges.
 */
void hamiltonian_cycle_graph(int   v,
                             int   e,
                             int   dir_flag,
                             Graph **out_graph)
{
   //int i, j, k, l, count, index, *adj_matrix, *ham;
   //FILE *fptr;

   //if ( ( adj_matrix = ( int * ) calloc( v * v, sizeof( int ) ) )
   //     == NULL ) {
   //   printf( "Not enough room for this size graph\n" );
   //   return;
   //}

   //if ( ( ham = ( int * ) calloc( v, sizeof( int ) ) ) == NULL ) {
   //   printf( "Not enough room for this size graph\n" );
   //   free( adj_matrix );
   //   return;
   //}

   //printf( "\n\tBeginning construction of graph.\n" );

   ///*  Generate a random permutation in the array ham. */
   //init_array( ham, v );
   //permute( ham, v );

   //if ( ( fptr = fopen( ham_file, "w" ) ) == NULL ) {
   //   printf( "\n\t\t Could not open file %s.\n", ham_file );
   //   free( adj_matrix );
   //   free( ham );
   //   return;
   //}

   ///* print Hamiltonian cycle and store required edges */
   //for ( i = 0; i < v; i++ ) {
   //   fprintf( fptr, "%5d\n", 1 + ham[ i ] );
   //   k = ham[ i ];
   //   l = ham[ ( i + 1 ) % v ];
   //   if ( k > l && !dir_flag )
   //      swap( &k, &l );
   //   adj_matrix[ k * v + l ] = 1;
   //}

   //fprintf( fptr, "%5d\n", 1 + ham[ 0 ] );
   //fclose( fptr );
   //free( ham );

   //for ( count = v; count < e; ) {
   //   if ( ( i = ran( v ) ) == ( j = ran( v ) ) )
   //      continue;
   //   if ( i > j && !dir_flag )
   //      swap( &i, &j );
   //   index = i * v + j;
   //   if ( !adj_matrix[ index ] ) {
   //      adj_matrix[ index ] = 1;
   //      count++;
   //   }
   //}

   //print_graph( v, count, out_file, adj_matrix, dir_flag );
}


/* This function generates a random network.  The user selects
 * the number of vertices (v) and edges (e), the maximum weight per edge, and the
 * the output file name. It is assumed that e <= v**2 - 3v + 3. (In this
 * program, this assured because of the call to fix_imbalanced_graph.) The
 * function will produce a network with at least e edges. In some rare,
 * cases, the actual number of edges may be slightly more than e because
 * of the following:
 *
 *    Each vertex is guaranteed to be on a directed path from the source
 *    to the sink. This is achieved by generating random directed paths
 *    from the source to the sink until every vertex is on such a path.
 */
void network_flow_graph(int   vertices,
                        int   edges,
                        int   max_weight,
                        Graph **out_graph)
{
   int start      = 0,
       finish     = vertices - 1,
       edge_count = 0,
       from, to,
   *adj_matrix,
   *closed_list;

   if (vertices == 1)
   {
      warning(SingleVertexNetwork);
      return;
   }

   /* adjacency matrix to represent graph */
   if ((adj_matrix = ( int * )calloc(vertices * vertices, sizeof(int))) == NULL)
   {
      warning(InsufficientStorage);
      return;
   }

   /* closed_list[ i ] == True if vertices[ i ] is on some path from start to finish */
   if ((closed_list = ( int * )calloc(vertices, sizeof(int))) == NULL)
   {
      warning(InsufficientStorage);
      free(adj_matrix);
      return;
   }

   /* put each vertex on some path from start to finish */
   closed_list[start] = True;
   do
   {
      /* from start to some vertex not already picked */
      to             = pick_to(closed_list, vertices);
      adj_matrix[to] = ran(max_weight) + 1;
      edge_count++;

      /* from current vertex to vertex != finish */
      if (to != finish)
      {
         from = to;
         closed_list[finish] = False;
         while ((to = pick_to(closed_list, vertices)) != finish)
         {
            adj_matrix[from * vertices + to] = ran(max_weight) + 1;
            edge_count++;
            from = to;
         }

         /* from vertex to finish */
         adj_matrix[from * vertices + finish] = ran(max_weight) + 1;
         edge_count++;
      }
   } while (unpicked_vertices_p(closed_list, vertices));

   /* add additional edges as needed */
   while (edge_count < edges)
   {
      from = ran(vertices - 1);
      to   = pick_to(NULL, vertices);
      if (!adj_matrix[from * vertices + to] && (from != to))
      {
         adj_matrix[from * vertices + to] = ran(max_weight) + 1;
         edge_count++;
      }
   }
   //print_graph( vertices, edge_count, outfile, adj_matrix, Directed );

   /* free storage */
   free(adj_matrix);
   free(closed_list);
}


int pick_to(int *closed, int vertices)
{
   int vertex;

   if (closed == NULL)          /* pick any vertex, even if picked before... */
   {
      do
      {
         vertex = ran(vertices);
      } while (vertex == 0);    /* ...except for start */
   }
   else
   {
      do
      {
         vertex = ran(vertices);
      } while (closed[vertex]);
      closed[vertex] = True;
   }
   return(vertex);
}


int unpicked_vertices_p(int *closed, int n)
{
   int i = 1;

   while (i < n)
   {
      if (!closed[i])
      {
         return(True);
      }
      else
      {
         i++;
      }
   }
   return(False);
}


/*** ran, etc. ***/
void seed_ran(unsigned int rand_seed)
{
   srand(rand_seed);
}


/* Return a random integer between 0 and k-1 inclusive. */
int ran(int k)
{
   return(rand() % k);
}


/* randomly permute a[ 0 ],...,a[ n - 1 ] */
void permute(int *a, int n)
{
   int i;

   for (i = 0; i < n - 1; i++)
   {
      swap(a + i + ran(n - i), a + i);
   }
}


void swap(int *a, int *b)
{
   int temp;

   temp = *a;
   *a   = *b;
   *b   = temp;
}


/* set a[ i ] = i, for i = 0,...,end - 1 */
void init_array(int *a, int end)
{
   int i;

   for (i = 0; i < end; i++)
   {
      *a++ = i;
   }
}


void create_graph(
   int   v,
   int   e,
   Graph **out_graph,
   int   *adj_matrix,
   int   dir_flag)
{
   int i, j, index;

   Graph *graph = new Graph();

   assert(graph != NULL);
   *out_graph = graph;

   vector<Graph::Vertex *> vertices;

   for (i = 0; i < v; i++)
   {
      Graph::Vertex *vertex = graph->addVertex();
      vertices.push_back(vertex);
   }
   if (!dir_flag)
   {
      for (i = 1; i < v; i++)
      {
         for (j = i + 1; j <= v; j++)
         {
            index = (i - 1) * v + j - 1;
            if (adj_matrix[index])
            {
               graph->connectVertices(vertices[i - 1], vertices[j - 1], false);
            }
         }
      }
   }
   else
   {
      for (i = 1; i <= v; i++)
      {
         for (j = 1; j <= v; j++)
         {
            index = (i - 1) * v + j - 1;
            if (adj_matrix[index])
            {
               graph->connectVertices(vertices[i - 1], vertices[j - 1], false);
            }
         }
      }
   }
}
