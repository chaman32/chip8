
#include "chip8.h"



// Display size
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

chip8 chip8App;



int main(int argc, char **argv)
{
	if (argc < 2)
	{
		cout << "Usage: myChip8.exe chip8application\n\n" << endl;
		return 1;
	}

	// Load game
	if (!chip8App.loadApplication(argv[1]))
		return 1;


	return 0;
}