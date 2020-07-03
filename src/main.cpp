#include "game.cpp"
//#include <chrono>
using namespace std;


int main() {
    KnCnGraph graph(7);
    Game game(graph);
    // Set to true for maker, false for breaker as the first player
    game.play(true);
}