#include <iostream>
#include <vector>
#include <set>
#include <queue>
using namespace std;


// TODO: This probably will be a god class. Arrange stuff after getting a working solution.

struct Edge {
    int src, dest;
};

// Reperesentation of a possible cycle.
struct Cycle {
    set<int> seen; // This is for utility reasons in BFS.
    vector<int> nodes; // This vector will store the order of the nodes.
};

class Graph {

public:
    vector<vector<int> > adjList;
    set<set<int> > winningSets;
    vector<Edge> edgeList;


    int size;
    Graph(int n) {
        adjList.resize(n);
        size = n;
    }


    void printGraph() {
        for (int i = 0; i < size; i++) {
            cout << i << "-->";
            for (int v : adjList[i]) {
                cout << v << " ";
            }
            cout << endl;
        }
    }


    // Build a Kn graph
    void populateAsCompleteGraph() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (j == i) continue;
                adjList[j].push_back(i);
            }
        }
    }


    /*
        The focus of this project is on Kn - Cn graphs, so we have to remove a cycle. For consistency reasons,
        we remove the "outer" cycle. This can be observed if you arrange the nodes and draw them to look like a circle.
    */
    void deleteCycle() {
        for (int i = 0; i < size; i++) {
            vector<int> newVector;
            for (int j = 0; j < size - 1; j++) {
                if (adjList[i][j] == (i + 1) % size || adjList[i][j] == (size + (i - 1) % size) % size) {
                    continue;
                }
                newVector.push_back(adjList[i][j]);
            }
            adjList[i] = newVector;
        }
    }


    /*
        Use BFS to find all the ham cycles by using the fact that starting node's neighbours should be the first and the last element
        in the cycle (excluding the starting point ofc). Apply this logic recursively to get all elements in a cycle. Solution is iterative
        to avoid any stack overflows in bigger (Kn - Cn) graphs.

        Time complexity : O(n!) (NP Complete problem).
    */
    void generateWinningSets() {
        queue<Cycle> q;
        Cycle init;
        init.seen.insert(0);
        init.nodes.push_back(0);
        q.push(init);
        while (q.front().nodes.size() != size) {
            Cycle curr = q.front();
            cout << curr.nodes.size() << endl;
            q.pop();
            for (int child : adjList[curr.nodes.back()]) {
                cout << child << " ";
                if (curr.seen.find(child) == curr.seen.end()) {
                    Cycle newCycle;
                    newCycle.nodes = curr.nodes;
                    newCycle.seen = curr.seen;
                    newCycle.seen.insert(child);
                    newCycle.nodes.push_back(child);
                    q.push(newCycle);
                }
            }
            cout << endl;
        }
        cout << q.size() << endl;
        int qsize = q.size();

        for (int i = 0; i < qsize; i++) {
            Cycle curr = q.front();
            q.pop();
            for (int node : adjList[0]) {
                if (node == curr.nodes.back()) {
                    for (int node : curr.nodes) {
                        cout << node + 1 << " ";
                    }
                    break;
                }
            }

            cout << endl;
        }



    }

};
