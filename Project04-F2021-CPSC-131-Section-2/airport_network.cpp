#include "airport_network.hpp"
#include "airport_database.hpp"
#include <cassert>
#include <string>
#include <utility>
#include <vector>

#include "adjacency_list_graph.hpp"
#include "adjacency_matrix_graph.hpp"
#include "airport_database.hpp"
#include "graph_traversal.hpp"
#include "undirected_graph.hpp"

AirportNetwork::AirportNetwork(const AirportDatabase& airport_database)
                            : airport_database_(airport_database), 
                              airport_graph_(airport_database.size())
// Implement the AirportNetwork constructor.
//
// Like every constructor, this should properly set all of the private data
// members of the class. You should do as much initialization as possible in the
// initializer list, though some will need to be done in the method body.

{
  // Iterate for every flight route of the airports in the database.
  for (FlightRoute flight : airport_database.routes()) {
    // Store the indexes of the airports with appropriate codes.
    int from = airport_database.index(flight.code_one());
    int to = airport_database.index(flight.code_two());

    // Store the Airport objects with appropriate codes.
    Airport from_airport = airport_database.airport(flight.code_one());
    Airport to_airport = airport_database.airport(flight.code_two());

    // Calculate and store the great-circle distance.
    int great_circle = from_airport.distance_miles(to_airport);
    // Add an edge to the undirected graph with appropriate weight.
    airport_graph_.add_edge(from, to, great_circle);
  }
}

int AirportNetwork::num_airports() const noexcept {
  return airport_graph_.vertex_count();
}

int AirportNetwork::num_flight_routes() const noexcept {
  return airport_graph_.edge_count();
}

std::vector<std::string> AirportNetwork::at_most_one_layover(
    const std::string& code) const {
  // Implement the at_most_one_layover function.
  //
  // HINT: This will require a modicum of business logic. The graph theory
  // part of the problem should be delegated to a method call in
  // graph_traversal.

  // Checks if code is an airport code is not in the database.
  if (!airport_database_.index(code)) {
    throw std::invalid_argument("The airport code is not in the database.");
  }
  
  // Store the vertex number for the airport code.
  int vertex_number = airport_database_.index(code);
  // Store returned from distance_at_most_two.
  std::vector<bool> at_most_two = distance_at_most_two(airport_graph_, vertex_number);
  // Create an empty std::vector to store airport codes.
  std::vector<std::string> codes;

  // Iterate through at_most_two.
  for (int i = 0; i < at_most_two.size(); ++i) {
    if (at_most_two[i]) { // If vertex is at most distance two from the start vertex.
      codes.push_back(airport_database_.code(i)); // Push this code to codes.
    }
  }
  return codes;
}

std::vector<int> AirportNetwork::least_distance(const std::string& code) const {
  // Implement the least_distance function.
  //
  // HINT: This will require a small amount of business logic. The graph theory
  // part of the problem should be delegated to a method call in
  // graph_traversal.

  // Checks if code is an airport code is not in the database.
  if (!airport_database_.index(code)) {
    throw std::invalid_argument("The airport code is not in the database.");
  }

  // Store the vertex number for the airport code.
  int vertex_number = airport_database_.index(code);
  // Call shortest_path and return the resulting std::vector.
  return shortest_path(airport_graph_, vertex_number);
}