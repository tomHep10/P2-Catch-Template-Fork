//#include <catch2/catch_test_macros.hpp>
#include "catch/catch_amalgamated.hpp"
#include <iostream>
#include "AdjacencyList.h"

TEST_CASE("Test 1: Add a single directed edge") {
    AdjacencyList graph;
    graph.addEdge("google.com", "gmail.com");

    REQUIRE(graph.hasEdge("google.com", "gmail.com") == true);
    REQUIRE(graph.hasEdge("gmail.com", "google.com") == false);
}

TEST_CASE("Test 2: Add node without edges") {
    AdjacencyList graph;
    graph.addNode("maps.com");

    REQUIRE(graph.hasEdge("maps.com", "google.com") == false);
}

TEST_CASE("Test 3: Add self-loop edge") {
    AdjacencyList graph;
    graph.addEdge("gmail.com", "gmail.com");

    REQUIRE(graph.hasEdge("gmail.com", "gmail.com") == true);
}

TEST_CASE("Test 4: Call PageRank with 2 iterations") {
    AdjacencyList graph;
    graph.addEdge("A", "B");
    graph.addEdge("B", "C");

    std::string result = graph.PageRank(2);

    REQUIRE(result == ""); 
}

TEST_CASE("Test 5: PageRank on disconnected node") {
    AdjacencyList graph;
    graph.addEdge("google.com", "gmail.com");
    graph.addNode("lonely.com");

    std::string result = graph.PageRank(3);

    REQUIRE(result == "");
}
