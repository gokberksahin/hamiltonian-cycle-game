#include <stdio.h>
#include "graph.cpp"

using namespace std;


int main() {
    Graph graph(6);
    graph.populateAsCompleteGraph();
    graph.deleteCycle();
    graph.printGraph();
    graph.generateWinningSets();

}