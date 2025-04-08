#pragma once

#include <vector>
#include <string>
#include <map>

using namespace std;

class AdjacencyList {
private:
    // maps for page to id and id to page
    map<string, int> page_to_id;
    map<int, string> id_to_page;

    // adjacency list and ranks, each use id's as keys using createID
    map<int, vector<int>> adj;
    map<int, double> ranks;

    // helps keep track of the number of unique pages
    int id = 0;

    // creates id's checks for duplicates
    // returns the id of the page if it exists, if not returns a new id sequentially 0-n
    int createID(const string& url);

    // Calculates the out-degree for every page
    map<int, double> calculateOutDegrees() const;

public:
    // adds pages to adjacency list, uses createID
    void addEdge(const string& from_url, const string& to_url);

    // does initial ranks, and then power iterations
    void calculatePageRank(int power_iterations);

    // sorts ranks alphabetically, prepares for output
    map<string, double> getSortedRanks() const;

    // prints the graph structure (for debugging)
    void printGraph() const;
};