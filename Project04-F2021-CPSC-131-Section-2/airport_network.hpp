#ifndef _airport_network_hpp_
#define _airport_network_hpp_

#include <string>
#include <utility>
#include <vector>

#include "adjacency_list_graph.hpp"
#include "adjacency_matrix_graph.hpp"
#include "airport_database.hpp"
#include "undirected_graph.hpp"

// The AirportNetwork class offers graph traversal algorithms over a database
// of airports and flights.
class AirportNetwork {
 public:
  // Constructs an AirportNetwork modeling the data in airport_database as a
  // weighted undirected graph.
  AirportNetwork(const AirportDatabase& airport_database);

  // Returns the number of airports in the network.
  int num_airports() const noexcept;

  // Returns the number of flight routes in the network.
  int num_flight_routes() const noexcept;

  // Returns the airport codes that are at most one layover away from `code`.
  //
  // Throws a std::invalid_argument exception if `code` is not an airport code
  // in the database.
  //
  // NOTE: As an example, this will equal one if there is a direct flight from
  // `from_code` to `to_code`.
  std::vector<std::string> at_most_one_layover(const std::string& code) const;

  // Returns the shortest path distance of travel (in miles) when flying from
  // `code` to each airport.
  //
  // Throws a std::invalid_argument exception if `code` is not an airport code
  // in the database.
  //
  // NOTE: If there is a direct flight from ` to `to_code`, this will
  // be the great-circle distance between the airports. If there is not a direct
  // flight, this will measure the amount one needs to deviate from the
  // great-circle route given the available flights.

  
  std::vector<int> least_distance(const std::string& code) const;

 private:
  // The database of airports.
  const AirportDatabase airport_database_;

  // A (undirected, weighted) graph modeling the airports in airport_database_.
  // * The airport with a given (three letter) IATA code is represented by
  //   vertex i, where i is airport_database_.index(code).
  // * An edge between vertex i and vertex j represents there being a flight
  //   from Airport i to Airport j.
  // * The weight of the edge (if present) between vertex i and vertex j
  //   is the (great-circle) distance between Airport i and Airport j.
  //
  // For more information about great-circle distance, see
  // https://en.wikipedia.org/wiki/Great-circle_distance.

  // Remember that we have two implementations of the Undirected Graph ADT:
  // using adjacency lists and using an adjacency matrix. Determine which is
  // the better choice for at_most_one_layover and least_distance, leaving
  // exactly one of the following lines commented out. Document your decision
  // (in a comment) with one or two sentences justifying your choice.

  UndirectedGraph<AdjacencyListGraph> airport_graph_;
  //UndirectedGraph<AdjacencyMatrixGraph> airport_graph_;

  /*
   * We chose to use an adjacency list because we do not use the
   * has_edge method for our implementation of at_most_one_layover and 
   * least_distance because has_edge is a bottleneck. Also, these methods
   * use the distance_at_most_two and shortest_path methods from the
   * graph_traversal file, and both of them have a call to out_edges which 
   * is typically more efficient with an adjacency list.
   */
};

#endif