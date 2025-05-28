#include "Game.h"
#include "SFML/Graphics.hpp"
int main() {
	srand(time(0));
	Game game("config.txt");
	game.run();
	return 0;
}