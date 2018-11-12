#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


unsigned char chip8_fontset[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, //0
	0x20, 0x60, 0x20, 0x20, 0x70, //1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
	0x90, 0x90, 0xF0, 0x10, 0x10, //4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
	0xF0, 0x10, 0x20, 0x40, 0x40, //7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
	0xF0, 0x90, 0xF0, 0x90, 0x90, //A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
	0xF0, 0x80, 0x80, 0x80, 0xF0, //C
	0xE0, 0x90, 0x90, 0x90, 0xE0, //D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
	0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

chip8::chip8()
{
	// empty
}

chip8::~chip8()
{
	// empty
}


void chip8::emulateCycle()
{
	// Fetch opcode
	opcode = memory[pc] << 8 | memory[pc + 1];

	// Decode opcode
	switch (opcode & 0xF000)
	{
		// Some opcodes //

	case 0xA000: // ANNN: Sets I to the address NNN
	  // Execute opcode
		I = opcode & 0x0FFF;
		pc += 2;
		break;

		// More opcodes //

	default:
		cout << "Unknown opcode: " << opcode << endl;

	}

	// Update timers
	if (delay_timer > 0)
		--delay_timer;

	if (sound_timer > 0)
	{
		if (sound_timer == 1)
			printf("BEEP!\n");
		--sound_timer;
	}
}

void chip8::init()
{

	pc = 0x200;		// Program counter starts at 0x200 (Start adress program)
	opcode = 0;			// Reset current opcode	
	I = 0;			// Reset index register
	sp = 0;			// Reset stack pointer

	// Clear display
	for (int i = 0; i < 2048; ++i)
		gfx[i] = 0;

	// Clear stack
	for (int i = 0; i < 16; ++i)
		stack[i] = 0;

	for (int i = 0; i < 16; ++i)
		key[i] = V[i] = 0;

	// Clear memory
	for (int i = 0; i < 4096; ++i)
		memory[i] = 0;

	// Load fontset
	for (int i = 0; i < 80; ++i)
		memory[i] = chip8_fontset[i];

	// Reset timers
	delay_timer = 0;
	sound_timer = 0;

	// Clear screen once
	drawFlag = true;

	srand(time(NULL));


}


bool chip8::loadApplication(const char * filename)
{
	init(); //http://www.cplusplus.com/doc/tutorial/files/

	ifstream readFileGame;

	readFileGame.open(filename, ifstream::in);

	if (!readFileGame.good())
	{
		cout << "File error" << endl;
		return false;
	}

	readFileGame.seekg(0, ios_base::end);
	long fSize = readFileGame.tellg();
	readFileGame.seekg(0);

	cout << "File size " << fSize << endl;

	auto buffer = new char[fSize];
 
	if (buffer == NULL)
	{
		cout << "Memory error" << endl;
		return false;
	}

	readFileGame.read(buffer, fSize);

	cout << "Byte: " << readFileGame.gcount() << endl;

	cout << "Loading: %s\n" << filename << endl;

	// Copy buffer to Chip8 memory
	if ((4096 - 512) > fSize)
	{
		for (int i = 0; i < fSize; ++i)
			memory[i + 512] = buffer[i];
	}
	else
		cout<< "Error: ROM too big for memory" << endl;

	readFileGame.close();
	delete buffer;

	return true;
}
