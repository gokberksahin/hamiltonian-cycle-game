#include <iostream>
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

    // Solve the game for the starting player and output the moves in order
    void play(bool startingPlayer) {
        BestMove startingMove;
        startingMove.availableWinningSets = winningSets;
        BestMove bestMoves = miniMax(startingPlayer, startingMove, 0);
        string firstMove = startingPlayer ? "Maker" : "Breaker";
        string winner = bestMoves.score == 1 ? "Maker" : "Breaker";
        cout << "Game has started with first player: " << firstMove << endl;
        cout << winner << " has won." << endl;
        cout << "Maker's moves were : " << endl;
        for (Edge move : bestMoves.makerEdges) {
            cout << move.src + 1 << " " << move.dest + 1 << endl;
        }
        cout << "Breaker's moves were : " << endl;
        for (Edge move : bestMoves.breakerEdges) {
            cout << move.src + 1 << " " << move.dest + 1 << endl;
        }
    }

    void printWinningSets() {
        int i = 1;
        for (const set<Edge>& winningSet : winningSets) {
            cout << "Edges for set " << i << endl;
            for (Edge e : winningSet) {
                cout << e.src + 1 << " " << e.dest + 1 << endl;
            }
            i++;
        }
    }


private:

    // Check if Maker has won
    bool hasMakerWon(const vector<Edge>& edges, const set<set<Edge>>& availableWinningSets ) {
        set<Edge> edgeSet(edges.begin(), edges.end());
        for (const set<Edge>& winningSet : availableWinningSets) {
            set<Edge> intersect;
            set_intersection(winningSet.begin(), winningSet.end(), edgeSet.begin(), edgeSet.end(), inserter(intersect, intersect.begin()));
            if (intersect.size() == winningSet.size()) {
                return true;
            }
        }
        return false;
    }

    // Remove the sets if Maker has claimed at least one edge from that set
    void removeWinningSet(set<set<Edge>> &availableWinningSets, Edge edge) {

        // TODO: This can be simplified with the iterator approach
        set<set<Edge>> willBeErased;

        for (set<Edge> winningSet : availableWinningSets) {
            if (winningSet.find(edge) != winningSet.end()) {
                willBeErased.insert(winningSet);
            }
        }

        for (const set<Edge>& toBeDiscardedSet : willBeErased) {
            availableWinningSets.erase(toBeDiscardedSet);
        }
    }

    bool isInWinningSet(set<set<Edge>> &availableWinningSets, Edge edge) {

        for (set<Edge> winningSet : availableWinningSets){
            if (winningSet.find(edge) != winningSet.end()) return true;
        }
        return false;
    }

    // Data structure for carrying the necessary data in recursion
    struct BestMove {
        vector<Edge> makerEdges;
        vector<Edge> breakerEdges;
        set<Edge> seen;
        set<set<Edge>> availableWinningSets;
        int score;
    };

    // Recursive minimax function for finding the optimal moves for both players
    BestMove miniMax(bool isMaximizer, BestMove &move, int depth) {

        // Check if there are any unclaimed winning sets for the maker to claim, if there are none there is no way for
        // Maker to win the game so Breaker has won the game, update the score and return the move
        if (move.seen.size() == gameBoard.size() || move.availableWinningSets.empty()) {
            move.score = -1;
            return move;
        }

        // Check if maker has won the game by intersecting all available winning sets to current claimed edges
        if (move.makerEdges.size() >= graph.size && hasMakerWon(move.makerEdges, move.availableWinningSets)) {
            move.score = 1;
            return move;
        }

        BestMove bestMove;

        for (Edge e : gameBoard) {
            if (move.seen.find(e) == move.seen.end() && isInWinningSet(move.availableWinningSets, e)) {

                BestMove nextMove = move; //Copy the current move so it doesn't effect sibling branches
                nextMove.seen.insert(e);

                if (isMaximizer) {
                    nextMove.makerEdges.push_back(e); // Maker claims the edge
                } else {
                    nextMove.breakerEdges.push_back(e);
                    // Remove all winning sets that contains current claimed edge by the Breaker
                    removeWinningSet(nextMove.availableWinningSets, e);
                }

                BestMove nextBestMove = miniMax(!isMaximizer, nextMove, depth + 1);

                // The outcome of the game is binary so we can leverage this fact to prune the tree
                if (isMaximizer && nextBestMove.score == 1) return nextBestMove;
                if (!isMaximizer && nextBestMove.score == -1) return nextBestMove;
                bestMove = nextBestMove;
            }
        }

        return bestMove;
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

            for (int node : graph.adjList[0]) {
                set<Edge> winningSet;
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