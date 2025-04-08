#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // For fixed and setprecision
#include "AdjacencyList.h"

using namespace std;

// Using example shown in project 2 breakdown video as inspiration for paring input
int main() {
    int n = 0, p = 0; // n = lines, p = power iterations
    cin >> n >> p;

    AdjacencyList graph;
    string from_page, to_page;

    // Project 2 breakdown video example input
    for ( int i = 0; i < n+1; i++)
    {
        string line;
        getline(cin, line);
        istringstream in(line);

        string from;
        in>>from;

        string to;
        in>>to;

        // skipping empty values from input
        if (from.empty() || to.empty()) {
            continue;
        }

        graph.addEdge(from, to);
    }

    // calculating ranks and sorting pages alphabetically
    graph.calculatePageRank(p);
    map<string, double> final_ranks = graph.getSortedRanks();

    // using project 2 breakdown video example output
    cout << fixed << showpoint;
    cout << setprecision(2);

    for (const auto& page_rank : final_ranks) {
        //cout << "page rank first: " << page_rank.first << " page rank second " << page_rank.second << endl;
        cout << page_rank.first << " " << page_rank.second << endl;
    }

    return 0;
}