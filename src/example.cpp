Okay, let's refactor your AdjacencyList class to use std::map<int, std::vector<int>> for the adjacency list instead of std::vector<std::vector<int>>.

Here's how the code changes:

1. Update the Header File (AdjacencyList.h)

You need to change the declaration of adj.

#pragma once

#include <vector>
#include <string>
#include <map>
#include <vector> // Include vector for the map's value type
#include <iostream> // For printing if needed in other methods
#include <algorithm> // May be needed for PageRank or sorting later

class AdjacencyList {
private:
    // maps for page to id and id to page
    std::map<std::string, int> page_to_id;
    std::map<int, std::string> id_to_page;

    // --- CHANGE HERE ---
    // Adjacency list using a map: Key is the 'from' node ID,
    // Value is a vector of 'to' node IDs.
    std::map<int, std::vector<int>> adj;
    // --- END CHANGE ---

    // stores ranks of pages (assuming this will be sized based on 'id')
    std::vector<double> ranks;

    // helps keep track of the number of unique pages (total vertices)
    int id = 0;

    // creates id's checks for duplicates
    // returns the id of the page if it exists, if not returns a new id sequentially 0-n
    int getOrCreateID(const std::string& page); // Changed 'url' to 'page' for consistency

    // Calculates the out-degree for every page
    std::vector<double> calculateOutDegrees() const;

public:
    // Constructor (optional but good practice)
    AdjacencyList() = default;

    // adds pages to adjacency list, uses getOrCreateID
    void addEdge(const std::string& from_page, const std::string& to_page); // Changed 'url' to 'page'

    // does initial ranks, and then power iterations
    void calculatePageRank(int power_iterations); // Declaration assumed needed

    // sorts ranks alphabetically, prepares for output
    std::map<std::string, double> getSortedRanks() const; // Declaration assumed needed

    // Utility to print graph (needs modification for map)
    void printGraph() const; // Declaration assumed needed

    // Getter for total number of vertices (useful)
    int getNumVertices() const { return id; }
};


2. Update the Implementation File (AdjacencyList.cpp or similar)

Modify the methods that interacted with the old adj vector.

#include "AdjacencyList.h" // Include your header
#include <iostream>       // For cout
#include <vector>
#include <string>
#include <map>

// --- getOrCreateID Method ---
// No longer needs to resize adj. The map handles key creation automatically.
int AdjacencyList::getOrCreateID(const std::string& page) {
    // if page doesn't exist, creates a new id
    if (page_to_id.find(page) == page_to_id.end()) {
        int current_id = id++; // Increment first, then assign is common, or assign then increment. id++ uses current then increments.
        page_to_id[page] = current_id;
        id_to_page[current_id] = page;

        // --- REMOVED ---
        // No need to resize adj map explicitly here.
        // std::map automatically handles adding new keys when accessed via operator[]
        // if (current_id >= static_cast<int>(adj.size())) {
        //      adj.resize(current_id+1);
        // }
        // --- END REMOVED ---

        // Optional: If you want every created node ID to exist as a key in the map
        // even if it has no outgoing edges yet, you can add this line:
        // adj[current_id]; // Creates an empty vector for this key if it doesn't exist

        std::cout << "Created new ID: " << current_id << " for page: " << page << std::endl; // Adjusted cout
        return current_id;
    } else { // page already exists
        std::cout << "Found existing ID: " << page_to_id[page] << " for page: " << page << std::endl; // Debugging cout
        return page_to_id[page];
    }
}

// --- calculateOutDegrees Method ---
// Needs to iterate differently and use the total number of IDs ('id')
// for the size of the result vector.
std::vector<double> AdjacencyList::calculateOutDegrees() const {
    int num_vertices = id; // Total number of unique vertices created
    std::vector<double> out_degrees(num_vertices, 0.0); // Initialize all out-degrees to 0

    // Iterate through the map entries (nodes that HAVE outgoing edges)
    for (const auto& pair : adj) {
        int from_id = pair.first;
        const std::vector<int>& neighbors = pair.second;

        // Ensure the ID is valid (should always be < num_vertices)
        if (from_id >= 0 && from_id < num_vertices) {
            out_degrees[from_id] = static_cast<double>(neighbors.size());
        } else {
            // This case should ideally not happen if IDs are managed correctly
             std::cerr << "Warning: Found node ID " << from_id
                       << " in adjacency map, but expected range is [0, "
                       << num_vertices << ")" << std::endl;
        }
    }

    // Debug printing (optional, can be adapted)
    std::cout << "Calculated out_degrees (size " << out_degrees.size() << "): ";
    for (int i = 0; i < num_vertices; ++i) {
        std::cout << "id " << i << ": " << out_degrees[i] << " ";
    }
    std::cout << std::endl;

    // Example specific debug print (adjust index if needed)
    if (num_vertices > 4) {
        std::cout << "out degree of ID 4: " << out_degrees[4] << std::endl;
    }


    return out_degrees;
}

// --- addEdge Method ---
// This method works almost identically with std::map.
// The operator[] on std::map will create a default-constructed
// std::vector<int> if the key `from_id` doesn't exist yet.
void AdjacencyList::addEdge(const std::string& from_page, const std::string& to_page) {
    int from_id = getOrCreateID(from_page);
    int to_id = getOrCreateID(to_page); // Ensure the 'to' page also gets an ID

    // If from_id is a new key, adj[from_id] creates an empty vector.
    // Then push_back adds the to_id to that vector.
    adj[from_id].push_back(to_id);
     std::cout << "Added edge from " << from_page << "(" << from_id << ") to "
               << to_page << "(" << to_id << ")" << std::endl; // Debugging cout
}

// --- Placeholder/Example Implementations for other methods ---
// You'll need to implement these based on your PageRank logic

void AdjacencyList::calculatePageRank(int power_iterations) {
    int num_vertices = id;
    if (num_vertices == 0) return;

    // Initialize ranks (e.g., uniformly)
    ranks.assign(num_vertices, 1.0 / num_vertices);
    std::vector<double> new_ranks(num_vertices);
    std::vector<double> out_degrees = calculateOutDegrees();

    double damping_factor = 0.85; // Common damping factor

    for (int iter = 0; iter < power_iterations; ++iter) {
        // Reset new_ranks for this iteration
        std::fill(new_ranks.begin(), new_ranks.end(), (1.0 - damping_factor) / num_vertices);

        // Calculate contributions from links
        // Iterate through nodes that have outgoing links (keys in adj map)
        for (const auto& pair : adj) {
            int from_id = pair.first;
            const std::vector<int>& neighbors = pair.second;
            double degree = out_degrees[from_id];

            if (degree > 0) { // If it's not a sink node in our map view
                double contribution = damping_factor * ranks[from_id] / degree;
                for (int to_id : neighbors) {
                    if (to_id >= 0 && to_id < num_vertices) { // Bounds check
                         new_ranks[to_id] += contribution;
                    }
                }
            }
        }

         // Handle sink nodes (nodes with out_degree 0) - distribute their rank equally
         double sink_rank_sum = 0.0;
         for(int i = 0; i < num_vertices; ++i) {
             if (out_degrees[i] == 0) {
                 sink_rank_sum += ranks[i];
             }
         }
         double sink_contribution = damping_factor * sink_rank_sum / num_vertices;
         for(int i = 0; i < num_vertices; ++i) {
             new_ranks[i] += sink_contribution;
         }


        // Update ranks for the next iteration
        ranks = new_ranks;

         std::cout << "Iteration " << iter + 1 << " complete." << std::endl;
         // Optional: Print ranks per iteration for debugging
         /*
         std::cout << "Ranks: ";
         for(double r : ranks) std::cout << r << " ";
         std::cout << std::endl;
         */
    }
     std::cout << "PageRank calculation finished after " << power_iterations << " iterations." << std::endl;
}


std::map<std::string, double> AdjacencyList::getSortedRanks() const {
    std::map<std::string, double> sorted_ranks;
    int num_vertices = id;

    if (ranks.size() != num_vertices) {
         std::cerr << "Error: Ranks vector size does not match number of vertices!" << std::endl;
         return sorted_ranks; // Return empty map on error
    }


    for (int i = 0; i < num_vertices; ++i) {
        // Use id_to_page map to find the page name for the current ID
        auto it = id_to_page.find(i);
        if (it != id_to_page.end()) {
            sorted_ranks[it->second] = ranks[i]; // map sorts automatically by key (page name)
        } else {
             std::cerr << "Warning: No page name found for ID " << i << std::endl;
        }
    }
    return sorted_ranks;
}


void AdjacencyList::printGraph() const {
     std::cout << "\n--- Adjacency List Representation ---" << std::endl;
     std::cout << "Total unique pages (vertices): " << id << std::endl;

     // Iterate through all known IDs from 0 to id-1
     for(int i = 0; i < id; ++i) {
         std::string page_name = "UNKNOWN";
         auto name_it = id_to_page.find(i);
         if (name_it != id_to_page.end()) {
             page_name = name_it->second;
         }
         std::cout << page_name << " (" << i << ") -> ";

         // Find if this ID exists as a key in the adjacency map
         auto adj_it = adj.find(i);
         if (adj_it != adj.end()) {
             // Node has outgoing edges
             const std::vector<int>& neighbors = adj_it->second;
             if (neighbors.empty()) {
                 std::cout << "[No outgoing links]";
             } else {
                 std::cout << "[ ";
                 for (size_t j = 0; j < neighbors.size(); ++j) {
                     int neighbor_id = neighbors[j];
                     std::string neighbor_name = "UNKNOWN";
                     auto neighbor_name_it = id_to_page.find(neighbor_id);
                     if(neighbor_name_it != id_to_page.end()) {
                         neighbor_name = neighbor_name_it->second;
                     }
                     std::cout << neighbor_name << "(" << neighbor_id << ")";
                     if (j < neighbors.size() - 1) {
                          std::cout << ", ";
                     }
                 }
                 std::cout << " ]";
             }
         } else {
             // Node has no outgoing edges (not a key in the map)
             std::cout << "[No outgoing links]";
         }
         std::cout << std::endl;
     }
     std::cout << "-------------------------------------\n" << std::endl;
}
IGNORE_WHEN_COPYING_START
content_copy
download
Use code with caution.
C++
IGNORE_WHEN_COPYING_END

Summary of Key Changes:

adj Declaration: Changed from std::vector<std::vector<int>> to std::map<int, std::vector<int>>.

getOrCreateID: Removed the adj.resize() logic. std::map handles key creation implicitly when using operator[] in addEdge.

addEdge: The code adj[from_id].push_back(to_id) now leverages std::map::operator[]. If from_id isn't a key, it creates a default std::vector<int> (empty vector) for that key, then push_back adds the to_id.

calculateOutDegrees:

Gets the total number of vertices from the id counter, not adj.size().

Initializes the out_degrees vector to the correct size (id) with all zeros.

Iterates through the key-value pairs (const auto& pair : adj) in the map. For each pair, the pair.first is the from_id and pair.second.size() is its out-degree.

Updates the out_degrees vector at the index corresponding to from_id. Nodes that were created but have no outgoing edges (and thus aren't keys in adj) will correctly retain their 0.0 out-degree.

Other Methods (calculatePageRank, printGraph etc.): Need to be adapted to iterate over the map (for (const auto& pair : adj)) when processing outgoing links, and potentially iterate from 0 to id-1 when needing to consider all nodes (e.g., initializing ranks, handling sink nodes). Also, use adj.find(key) instead of index access if you need to check if a node has any outgoing links recorded.

This refactoring makes the adjacency list potentially sparse (only storing entries for nodes that actually have outgoing edges) and removes the need for explicit resizing based on the maximum ID encountered. Remember to implement or adapt the remaining methods (calculatePageRank, getSortedRanks, printGraph) according to this new map-based structure.