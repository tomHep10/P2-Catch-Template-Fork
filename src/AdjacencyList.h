#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>

class AdjacencyList {
public:
    void addEdge(const std::string& from, const std::string& to);
    void addNode(const std::string& node);
    bool hasEdge(const std::string& from, const std::string& to) const;
    int getOutDegree(const std::string& node) const;

    void runPageRank(int iterations);  // Stub for test compatibility
    std::map<std::string, double> getPageRanks() const;  // Stub for test compatibility
    std::string PageRank(int iterations);  // Your actual stub implementation

private:
    std::unordered_map<std::string, std::unordered_set<std::string>> adjacencyList;
    std::unordered_set<std::string> allNodes;
    std::map<std::string, double> ranks;  // optional: placeholder
};
