#include "game.cpp"
//#include <chrono>
using namespace std;


int main() {
    KnCnGraph graph(5);
    Game game(graph);
    //auto start = chrono::high_resolution_clock::now();
    game.play(true);
    //auto stop = chrono::high_resolution_clock::now();
    //auto duration =  chrono::duration_cast<chrono::microseconds>(stop - start);
    //cout << "Finished in " << duration.count() << endl;
}