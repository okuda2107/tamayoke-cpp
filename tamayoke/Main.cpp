#include "TamayokeGame.h"
#include "game/base/IGame.h"

int main() {
    TamayokeGame* game = new TamayokeGame();
    IGame* interfaceGame = new IGame();
    interfaceGame->input = game;
    return 0;
}
