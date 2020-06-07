#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
#include "kn_cn_graph.cpp"


using namespace std;

class Game {

public:

    vector<Edge> gameBoard;
    KnCnGraph graph = NULL;
    set<set<Edge> > winningSets;

    Game(KnCnGraph& graph) {
        this->graph = graph;
        gameBoard = graph.edges;
        generateWinningSets();
    }

    void play(bool startingPlayer) {
        BestMove startingMove;
        BestMove bestMoves = miniMax(startingPlayer, startingMove, 0);
        string firstMove = startingPlayer ? "Maker" : "Breaker";
        string winner = bestMoves.score == 1 ? "Maker" : "Breaker";
        cout << "Game has started with first player: " << firstMove << endl;
        cout << winner << " has won." << endl;
    }


private:


    void printMoves(vector<Edge> makerMoves, vector<Edge> breakerMoves) {
        int i = makerMoves.size();
        int j = breakerMoves.size();
    }


    // Check if Maker has won
    bool hasMakerWon(const vector<Edge>& edges) {
        set<Edge> edgeSet(edges.begin(), edges.end());
        for (const set<Edge>& winningSet : winningSets) {
            set<Edge> intersect;
            set_intersection(winningSet.begin(), winningSet.end(), edgeSet.begin(), edgeSet.end(), inserter(intersect, intersect.begin()));
            if (intersect.size() == winningSet.size()) {
                return true;
            }
        }
        return false;
    }

    struct BestMove {
        vector<Edge> makerEdges;
        vector<Edge> breakerEdges;
        set<Edge> seen;
        int score;
    };

    BestMove miniMax(bool isMaximizer, BestMove move, int depth) {

        //cout << "depth at " << depth << endl;

        if (move.makerEdges.size() >= graph.size - 1 && hasMakerWon(move.makerEdges)) {
            move.score = 1;
            return move;
        }

        // if winningsete.empty() return move move.score -1
        if (move.makerEdges.size() + move.breakerEdges.size() == gameBoard.size()) {
            move.score = -1;
            return move;
        }

        BestMove bestMove;

        for (Edge e : gameBoard) {
            if (move.seen.find(e) == move.seen.end()) {
                move.seen.insert(e);
                BestMove nextMove = move;
                // copy winning set
                // winnings set - e
                if (isMaximizer) {
                    nextMove.makerEdges.push_back(e);
                } else {
                    nextMove.breakerEdges.push_back(e);
                }

                BestMove nextBestMove = miniMax(!isMaximizer, nextMove, depth + 1);
                if (isMaximizer && nextBestMove.score == 1) return nextBestMove;
                if (!isMaximizer && nextBestMove.score == -1) return nextBestMove;
                bestMove = nextBestMove;
            }
        }

        return bestMove;
    }

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


    // Reperesentation of a possible cycle.
    struct Cycle {
        set<int> seen; // This is for utility reasons in BFS.
        vector<int> nodes; // This vector will store the order of the nodes.
    };


    /*
        Use BFS to find all the ham cycles by using the fact that starting node's neighbours should be the first and the last element
        in the cycle (excluding the starting point ofc). Apply this logic recursively to get all elements in a cycle.

        Time complexity : O(n!)
    */
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
    }

};