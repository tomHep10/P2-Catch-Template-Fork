#include "AdjacencyList.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

int AdjacencyList::createID(const string& page) {
    // if page doesn't exist, creates a new id
    if (page_to_id.find(page) == page_to_id.end()) {
        int current_id = id++;
        page_to_id[page] = current_id;
        id_to_page[current_id] = page;

        return current_id;
    } else { // pages exists
        return page_to_id[page];
    }
}

// calculates out degrees based off size of adjacency list at node_id
map<int,double> AdjacencyList::calculateOutDegrees() const {
    map<int, double> out_degrees;

    for (const auto& [node_id, neighbors] : adj) {
        out_degrees[node_id] = neighbors.size();
    }

    return out_degrees;
}


// creating edge with createID and adding to adjacency list
void AdjacencyList::addEdge(const string& from_page, const string& to_page) {
    int from_id = createID(from_page);

    if (to_page.empty()) {
        if (adj.find(from_id) == adj.end()) {
            adj[from_id] = {};
        }
    } else {
        int to_id = createID(to_page);
        adj[from_id].push_back(to_id);
    }
}

void AdjacencyList::calculatePageRank(int power_iterations) {
    int nodes = id;
    if (nodes == 0) return;

    map<int, double> old_ranks;
    map<int, double> out_degrees = calculateOutDegrees();

    // for debugging adjacency list
    /*
    cout << "Adjacency List:" << endl;
    for (const auto& [node_id, neighbors] : adj) {
        cout << "Node " << id_to_page.at(node_id) << " -> { ";
        for (const auto& neighbor_id : neighbors) {
            cout << "(" << id_to_page.at(neighbor_id) << " ";
        }
        cout << "}" << endl;
    }
    */

    // debugging out_degrees
    /*
    // Print out the keys and values of out_degrees
    cout << "Out Degrees:" << endl;
    for (const auto& [key, value] : out_degrees) {
        cout << "Node " << key << ": " << value << endl;
    }
    */

    for (int p = 0; p < power_iterations; ++p) {
        if (p == 0) {
            // Initialize ranks to 1/n
            for (int j = 0; j < nodes; ++j) {
                ranks[j] = 1.0 / nodes;
            }
            old_ranks = ranks;

            if (p != power_iterations - 1) {
                for (int j = 0; j < nodes; ++j) {
                    ranks[j] = 0.0;
                }
            }
            continue;
        }

        for (int j = 0; j < nodes; ++j) {
            if (out_degrees[j] > 0) {
                for (const auto& k : adj[j]) {
                    ranks[k] += old_ranks[j] / out_degrees[j];
                    //debugging calculation
                    /*
                    cout << "calculating for " << id_to_page.at(j)
                            << " k=" << k
                            << " j=" << j
                            << " old_ranks[j]=" << old_ranks[j]
                            << " out_degrees[j]=" << out_degrees[j]
                            << " contribution of j to k rank += " << old_ranks[j] / out_degrees[j] << endl;
                    */
                }
            } else {
                continue;
            }
        }
        old_ranks = ranks;
        // if not at last iteration, reset the ranks
        if (p != power_iterations - 1) {
            for (int j = 0; j < nodes; ++j) {

                ranks[j] = 0.0;
            }
        }

    }
}

// sort ranks alphabetically
map<string, double> AdjacencyList::getSortedRanks() const {
    // automatically sorts strings alphabetically
    map<string, double> sorted_results;

    // find page name using id_to_page for all matching id's and ranks in ranks and add to results map
    for (const auto& id_rank : ranks) {
        int page_id = id_rank.first;
        double rank_score = id_rank.second;

        string page_name = id_to_page.at(page_id);

        sorted_results[page_name] = rank_score;
    }

    return sorted_results;
}
