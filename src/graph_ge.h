/*               A Graph Generation Package
 *          Richard Johnsonbaugh and Martin Kalin
 * Department of Computer Science and Information Systems
 *                    DePaul University
 *                   Chicago, IL  60604
 *        johnsonbaugh@cs.depaul.edu, kalin@cs.depaul.edu
 *
 *   This program generates graphs of specified  sizes  and
 * properties. Among the types of graphs are
 *
 * random graphs
 * random connected graphs
 * random directed acyclic graphs
 * random complete weighted graphs
 * random pairs of isomorphic regular graphs
 * random graphs with Hamiltonian cycles
 * random networks
 *
 * Graphs  may be specified further with respect to one or more
 * of these properties:
 *
 * weighted or unweighted
 * directed or undirected
 * simple or nonsimple
 *
 * We donate the program freely and hope others find it useful. We only
 * request attribution whenever this program is (successfully) shared. In
 * the spirit of the million dollar commercial vendors, we assume absolutely
 * no responsibility whatsoever for whatever mayhem this program
 * may inflict on your own system. Good luck!
 *
 *   The program is menu-driven. The top-level menu is
 *
 *   0. Exit Program
 *   1. Random Graph
 *   2. Random Digraph
 *   3. Random Connected Graph
 *   4. Random DAG (Directed Acyclic Graph)
 *   5. Random Complete Weighted Graph
 *   6. Random Isomorphic Graph Pair
 *   7. Random Hamiltonian Cycle Graph
 *   8. Random Network
 *   Choice?
 *
 * Selecting  any  choice  except  0  brings  a  sub-menu.  For
 * example,  if  the  user  selects  1,  a  series  of  prompts
 * results:
 *
 *        Number of Vertices:
 *
 *        Number of Edges:
 *
 *        1. Not Simple
 *        2. Simple
 *        Choice?
 *
 *        1. Unweighted Edges
 *        2. Weighted Edges
 *        Choice?
 *
 *        File for graph:
 *
 * A  nonsimple  graph  is  allowed  to  contain parallel edges
 * and/or  loops.  If  the  user  selects  the  Weighted  Edges
 * option, the prompt
 *
 *   Maximum Edge Weight:
 *
 * allows  the  user  to  enter a maximum edge weight. A random
 * graph  meeting  the  chosen specifications is then generated
 * and  written to the file named by the user. Output takes the
 * form
 *
 * n     m
 * v1    w1    c1
 * v2    w2    c2
 *    .
 *    .
 *    .
 *
 * where  n  is  the  number of vertices and m is the number of
 * edges  in  the  graph. Edge i is incident on vertices vi and
 * wi.  The weight of edge i is ci. If the graph is unweighted,
 * the value of ci is 1 for all i.
 *
 *   Other  options work similarly. For example, if the user
 * selects 2, Random Digraph, an output line
 *
 *   vi    wi    ci
 *
 * denotes  an  edge  directed  from  vi  to wi with weight ci.
 * Random  graphs  and digraphs are useful as input to programs
 * that  find components, that test for connectedness, and that
 * find   spanning   forests. Programs  that  find
 * spanning  trees  or minimal spanning trees can use
 * as  input  random  connected graphs, provided as option 3 on
 * the  top-level  menu.  Option 3 allows the user to designate
 * the connected graph's edges as weighted or unweighted.
 *
 *   Directed  acyclic graphs, suitable as input to programs
 * that  find  topological orderings, are provided by
 * option  4  in  the  top-level  menu.  The program writes the
 * generated  graph to one file and and a topological sort to a
 * second  file.  The  user  may specify the number of vertices
 * and  edges,  the  maximum  weight per edge, and the two file
 * names.
 *
 *   Complete   weighted  graphs  are  useful  as  input  to
 * programs   that   either   solve   exactly   the   traveling
 * salesperson  problem  or  approximate  an  exact solution to
 * this  problem. Option  5 generates such graphs,
 * allowing  the  user  to  specify  the  number  of  vertices,
 * whether  the graph is directed, the maximum weight per edge,
 * and the file name.
 *
 *   Pairs  of  isomorphic  graphs,  suitable  as  input  to
 * programs  that  test  for  graph  isomorphism, are
 * provided  by  option  6  in  the top-level menu. The program
 * writes  two  isomorphic,  regular,  undirected graphs (i.e.,
 * isomorphic,  undirected  graphs in which all of the vertices
 * have   the   same   degree)  to  two  files  and  writes  an
 * isomorphism  to  a  third  file. Pairs of isomorphic regular
 * graphs  make  challenging  input  to  programs that test for
 * graph  isomorphism. Under this
 * option,  the  user  specifies  the  number  of vertices, the
 * common vertex degree, and the file names.
 *
 *   Graphs  with  Hamiltonian cycles are useful as input to
 * programs  that  find  such  cycles or determine that they do
 * not  exist. Option  7  on  the  top-level  menu
 * provides  graphs with Hamiltonian cycles. The program writes
 * the  graph  to  one  file and a Hamiltonian cycle to another
 * file.  The  user may choose the number of vertices and edges
 * as well as the file names for the graph and the cycle.
 *
 *   Networks,  provided  as option 8 on the top-level menu,
 * are  suitable  as  input to programs that find maximal flows.
 * Under  this  option,  the program generates a
 * simple, weighted, directed graph satisfying the following:
 *
 * (a)  Vertex 1 has no incoming edges.
 *
 * (b)  Vertex  n,  where n is equal to the number of vertices,
 *   has no outgoing edges.
 *
 * The  user  selects  the  number  of  vertices and edges, the
 * maximum weight per edge, and the file name.
 *
 *   This program does some error and integrity checking. As
 * examples,  the  user  may  not  specify  a  graph  with zero
 * vertices  or  edges.  If  a  simple, undirected graph with n
 * vertices  is  to  be  generated,  the user should specify at
 * most  n(n - 1)/2  edges. If a Hamiltonian cycle graph with n
 * vertices  is  to  be  generated,  the user should specify at
 * least  n  edges.  If  a  network  with  n  vertices is to be
 * generated,  the  user  should  specify  at  most n*n - 3n + 3
 * edges.  The program corrects erroneous user input wherever
 * feasible.  In the examples just cited, the program would set
 * the edge count correctly if the user did not.
 *
 * For a full discussion, see the article by Johnsonbaugh
 * and Kalin, "A Graph Generation Software Package,"
 * in Proceedings of the 22nd SIGCSE Technical Symposium.
 */

#ifndef __GRAPH_GE__
#define __GRAPH_GE__

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "graph.hpp"

void seed_ran(unsigned int rand_seed);
void network_flow_graph(int   vertices,
                        int   edges,
                        int   max_weight,
                        Graph **out_graph);
void hamiltonian_cycle_graph(int   v,
                             int   e,
                             int   dir_flag,
                             Graph **out_graph);
void complete_graph(int   v,
                    int   max_wgt,
                    int   dir_flag,
                    Graph **out_graph);
void directed_acyclic_graph(int   v,
                            int   e,
                            int   max_wgt,
                            int   weight_flag,
                            Graph **out_graph);
void random_digraph(int   v,
                    int   e,
                    int   max_wgt,
                    int   weight_flag,
                    int   simple_flag,
                    Graph **out_graph);
void random_graph(int   v,
                  int   e,
                  int   max_wgt,
                  int   weight_flag,
                  int   simple_flag,
                  Graph **out_graph);
void random_connected_graph(int   v,
                            int   e,
                            int   max_wgt,
                            int   weight_flag,
                            Graph **out_graph);
void isomorphic_graph_pair(int   vertices,
                           int   spec_degree,
                           Graph **first_graph,
                           Graph **second_graph);
void regular_graph(int   vertices,
                   int   spec_degree,
                   Graph **out_graph);
#endif
