#include "graph_traversal.hpp"
#include "adjacency_list_graph.hpp"
#include <cassert>
#include <iostream>
#include <limits>
#include <vector>
#include <queue>

// A helper method for shortest_path that returns the index of the vertex that
// should be the next current node, given which nodes have already been visited
// and the tentative distances to nodes.
//
// The return value indicates whether the algorithm should continue running. If
// true, the next current index is returned through the reference parameter
// next_current.
bool next_current_for_shortest_path(
    const std::vector<bool>& visited, const std::vector<int>& distance,
    int& next_current) {
  // Assert the visited and distance vectors are the same size. If they are not,
  // a programming mistake has been made.
  assert(visited.size() == distance.size());

  // Initialize next_current_distance to be the (tentative) distance to some
  // (reachable) vertex that has not yet been visited.
  int next_current_distance = std::numeric_limits<int>::max();
  for (int v = 0; v < visited.size(); ++v) {
    if (!visited[v] && distance[v] < std::numeric_limits<int>::max()) {
      next_current = v;
      next_current_distance = distance[v];
      break;
    }
  }

  // Return false if next_current_distance wasn't set to something smaller than
  // std::numeric_limits<int>::max(). For Djikstra's algorithm, this happens
  // when we have visited every node reachable from the start vertex.
  if (next_current_distance == std::numeric_limits<int>::max()) {
    return false;
  }

  // Iterate through all the vertices, looking for the unvisited node with the
  // smallest (tentative) distance. As we find better candidates, update
  // next_current and next_current_distance.
  for (int v = 0; v < visited.size(); ++v) {
    if (!visited[v] && distance[v] < next_current_distance) {
      next_current = v;
      next_current_distance = distance[v];
    }
  }

  // Return true, as we have set next_current to an appropriate index.
  return true;
}

template <class Graph>
std::vector<bool> distance_at_most_two(const Graph& graph, const int start) {  
  // Implement the distance_at_most_two method.
  //
  // There are many different valid and good strategies to implement this
  // method.
  //
  // * It is possible to modify breadth first search, keeping track of how many
  //   edges have been traversed.
  // * It is possible to compute the vertices adjacent to the start vertex and
  //   the vertices adjacent to those vertices.
  // * It is possible to use shortest_path on a different graph.
  
  
  // * The vertex i is vertex start, i.e., i == start. In this case, vertex i is
  //   distance zero from vertex start.
  // * There is an edge from vertex start to vertex i. In this case, vertex i is
  //   distance one from vertex start.
  // * There is another vertex j with an edge from vertex start to vertex j and
  //   an edge from vertex j to vertex i. In this case, vertex i is distance two
  //   from vertex start.

  // Checks if the start vertex is valid.
  if (start >= graph.vertex_count() || start < 0) {
    throw std::range_error("Start is not a valid vertex");
  }

  // Create a vector to keep track of vertices visited.
  std::vector<bool> seen = std::vector<bool>(graph.vertex_count(), false);
  // Mark start as seen.
  seen[start] = true;
  
  for (const int i : graph.out_edges(start)) {
    seen[i] = true; // Mark every out-edge vertex i of start as seen.
    for (const int j : graph.out_edges(i)) {
      seen[j] = true; // Mark every out-edge vertex j of i as seen. 
    }
  }
  return seen;
}


template <class Graph>
std::vector<int> shortest_path(const Graph& graph, const int start) {  
  // Implement Djikstra's algorithm for finding the shortest paths from a given
  // vertex. For more information and as a reference, see
  // https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm.
  // 
  // START(Djikstra's Algorithm)
  //
  // PROCEDURE shortest_path(const Graph& graph, const int start)
  //
  //   Throw a std::invalid_argument exception if start is not a valid vertex.
  //
  //   Create a vector to track whether each node has been visited. Initialize
  //   it by setting all nodes as not visited.
  
  //
  //   Create a vector to track the shortest distance to each vertex from the
  //   start vertex. Initialize it by setting the start node as distance 0 and
  //   other nodes as distance infinity, which we represent by
  //   std::numeric_limits<int>::max().
  //
  //   Create a variable to track the current node being processed. Initialize
  //   it by setting it to be the start vertex.
  //
  //   WHILE (next_current_for_shortest_path)
  //
  //     For every out-edge of the current vertex, determine whether the path
  //     through the current vertex is shorter than the best path seen so far.
  //     If it is better, update its shortest distance.
  //
  //     Mark the current vertex as visited.
  //
  //  END WHILE
  //
  //  Return the vector of shortest distances.
  
  //
  // END PROCEDURE
  //
  // STOP

  // Checks if the start vertex is valid.
  if (start >= graph.vertex_count() || start < 0) {
    throw std::range_error("Start is not a valid vertex");
  }

  // Create a vector to keep track of vertices visited.
  std::vector<bool> seen = std::vector<bool>(graph.vertex_count(), false);
  // Create a vector to keep track of shortest distances.
  std::vector<int> shortest = std::vector<int>(graph.vertex_count(),
    std::numeric_limits<int>::max());
  // Set the starting vertex's distance as 0.
  shortest[start] = 0;
  // Declare a variable equal to the starting vertex.
  int current_vertex = start;

  while (next_current_for_shortest_path(seen, shortest, current_vertex)) {
    // Iterate for every out-edge of the current vertex.
    for (const int v : graph.out_edges(current_vertex)) {
      // Store the current edge's weight to be more time efficient.
      int edge_weight = graph.edge_weight(current_vertex, v);
      // If the current vertex is shorter than the best path seen so far.
      if (edge_weight + shortest[current_vertex] < shortest[v]) {
        shortest[v] = edge_weight + shortest[current_vertex]; // Update shortest.
      }
    }
    seen[current_vertex] = true; // Mark the current vertex as seen.
  }
  return shortest;
}

// Since the implementation of distance_at_most_two is in the cpp file, we need
// to tell the compiler which template instantiations to make.
//
// For more information, see
// https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl.
template std::vector<bool> distance_at_most_two<AdjacencyListGraph>(
    const AdjacencyListGraph& graph, const int start);
template std::vector<bool> distance_at_most_two<AdjacencyMatrixGraph>(
    const AdjacencyMatrixGraph& graph, const int start);
template std::vector<bool>
distance_at_most_two<UndirectedGraph<AdjacencyListGraph>>(
    const UndirectedGraph<AdjacencyListGraph>& graph, const int start);
template std::vector<bool>
distance_at_most_two<UndirectedGraph<AdjacencyMatrixGraph>>(
    const UndirectedGraph<AdjacencyMatrixGraph>& graph, const int start);

// Since the implementation of shortest_path is in the cpp file, we need to tell
// the compiler which template instantiations to make.
//
// For more information, see
// https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl.
template std::vector<int> shortest_path<AdjacencyListGraph>(
    const AdjacencyListGraph& graph, const int start);
template std::vector<int> shortest_path<AdjacencyMatrixGraph>(
    const AdjacencyMatrixGraph& graph, const int start);
template std::vector<int>
shortest_path<UndirectedGraph<AdjacencyListGraph>>(
    const UndirectedGraph<AdjacencyListGraph>& graph, const int start);
template std::vector<int>
shortest_path<UndirectedGraph<AdjacencyMatrixGraph>>(
    const UndirectedGraph<AdjacencyMatrixGraph>& graph, const int start);