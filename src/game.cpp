#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include "kn_cn_graph.cpp"


using namespace std;

class Game {

public:

    vector<Edge> gameBoard;
    KnCnGraph graph = NULL;
    set<set<Edge> > winningSets;

    Game(KnCnGraph& graph) {
        this->graph = graph;
        generateWinningSets();
    }



    // Check for if Maker has won,
    bool checkWinner(set<set<Edge>> sets, vector<Edge> gameBoard) {

        for (set<Edge> winningSet : sets) {

        }

    }

    void miniMax() {


    }

private:

    void printWinningSets() {
        int i = 1;
        for (const set<Edge>& winningSet : winningSets) {
            cout << "Edges for set " << i << endl;
            for (Edge e : winningSet) {
                cout << e.src << " " << e.dest << endl;
            }
            i++;
        }
    }

    /*
        Use BFS to find all the ham cycles by using the fact that starting node's neighbours should be the first and the last element
        in the cycle (excluding the starting point ofc). Apply this logic recursively to get all elements in a cycle.

        Time complexity : O(n!)
    */

    // Reperesentation of a possible cycle.
    struct Cycle {
        set<int> seen; // This is for utility reasons in BFS.
        vector<int> nodes; // This vector will store the order of the nodes.
    };

    void generateWinningSets() {

        queue<Cycle> q;
        Cycle init;
        init.seen.insert(0);
        init.nodes.push_back(0);
        q.push(init);
        while (q.front().nodes.size() != graph.size) {
            Cycle curr = q.front();
            q.pop();
            for (int child : graph.adjList[curr.nodes.back()]) {
                if (curr.seen.find(child) == curr.seen.end()) {
                    Cycle newCycle;
                    newCycle.nodes = curr.nodes;
                    newCycle.seen = curr.seen;
                    newCycle.seen.insert(child);
                    newCycle.nodes.push_back(child);
                    q.push(newCycle);
                }
            }
        }
        int qsize = q.size();

        for (int i = 0; i < qsize; i++) {
            Cycle curr = q.front();
            q.pop();
            set<Edge> winningSet;
            for (int node : graph.adjList[0]) {
                if (node == curr.nodes.back()) {
                    for (int j = 1; j < curr.nodes.size(); j++) {
                        winningSet.insert(Edge(curr.nodes[j - 1], curr.nodes[j]));
                    }
                    winningSet.insert(Edge(node, 0));
                    winningSets.insert(winningSet);
                    break;
                }
            }
        }
        printWinningSets();


    }

};