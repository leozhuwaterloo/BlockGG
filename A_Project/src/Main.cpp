#include "./managers/includes/GameManager.hpp"

#include <iostream>

int main(int argc, char **argv) {
  CS488Window::launch(argc, argv, new GameManager(), 1280, 720, "BlockGG");
	return 0;
}
