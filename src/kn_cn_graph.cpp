#include <iostream>
#include <vector>
#include <set>
#include "edge.cpp"
using namespace std;


class KnCnGraph {

private:
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

    void generateAllEdges() {
        for (int i = 0; i < size - 2; i++) {
            edges.emplace_back(i , i + 2);
            if (i + 3 < size)
                edges.emplace_back(i , i + 3);
            if (i + 4 < size)
                edges.emplace_back(i , i + 4);
        }
    }

public:
    vector<vector<int> > adjList;
    vector<Edge> edges;


    int size;
    KnCnGraph(int n) {
        adjList.resize(n);
        size = n;
        populateAsCompleteGraph();
        deleteCycle();
        generateAllEdges();
    }


    void printGraph() {
        cout << "Graph : " << endl;
        for (int i = 0; i < size; i++) {
            cout << i << "-->";
            for (int v : adjList[i]) {
                cout << v << " ";
            }
            cout << endl;
        }
    }

    void printEdges() {
        cout << "Graph edges:" << endl;
        for (Edge e : edges) {
            cout << e.src << "<->" << e.dest << endl;
        }
    }



};
