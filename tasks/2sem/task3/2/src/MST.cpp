//
// Created by valentiay on 4/21/17.
//

#include "../include/MST.h"

void MST(const Graph &graph, Graph & result)
{
    const unsigned INFINITY = 2147483647;
    // Stores pair<adjacent used vertex, weight of edge> for every edge
    vector<pair_ud> key(graph.getSize(), pair_ud(INFINITY, -1));
    // Stores true if [i] vertex was used in MST
    vector<bool> used(graph.getSize(), false);
    // Priority queue
    set<pair_du> queue;

    // Initializing keys
    key[0].second = 0;
    for(int i = 0; i < graph.getSize(); i++)
        queue.emplace(pair_du(key[i].second, i));

    // Prim's algorithm
    while(!queue.empty()){
        // Extracting minimum
        unsigned v = queue.begin()->second;
        queue.erase(queue.begin());
        used[v] = true;
        // Relaxing related vertices
        vector<pair_ud> related;
        graph.getRelated(v, related);
        for(const pair_ud & u : related){
            if(!used[u.first] && key[u.first].second > u.second
               || key[u.first].second + 1 < 0.000001){
                // Decreasing key
                queue.erase(pair_du(key[u.first].second, u.first));
                key[u.first].first = v;
                key[u.first].second = u.second;
                queue.emplace(key[u.first].second , u.first);
            }
        }
    }

    for(unsigned i = 1; i < graph.getSize(); i++)
        result.addEdge(i, key[i].first, key[i].second);
}