#pragma once

#include <vector>
#include <string>
#include <map>

using namespace std;

class AdjacencyList {
private:
    int id = 0;

    // maps for page to id and id to page
    map<string, int> page_to_id;
    map<int, string> id_to_page;

    // adjacency list and ranks, each use id's as keys using createID
    map<int, vector<int>> adj;
    map<int, double> ranks;

    map<int, double> calculateOutDegrees() const; // finds out degrees of every page

    // creates id's and checks for duplicates id's
    int createID(const string& url);


public:
    void calculatePageRank(int power_iterations); // does initial ranks, and then power iterations
    void addEdge(const string& from_url, const string& to_url); // adds pages to adjacency list, uses createID
    map<string, double> getSortedRanks() const; // sorts ranks alphabetically, prepares for output
};