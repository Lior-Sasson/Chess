#include "CLI.h"
#include "GUI.h"

int main(int argc, char *argv[]) {
	if (argc == 2) {
		if (strncmp(argv[1], "-g", 2) == 0) {
			runGame(); // GUI
			return 0;
		}
		else if (strncmp(argv[1], "-c", 2) == 0) {
			playGame(); // Console
			return 0;
		}
	}
	else if (argc == 1)
		playGame();
	else
		printf("invalid arguments.");
	return 0;
}
