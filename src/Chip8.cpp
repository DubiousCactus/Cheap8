/*
 * Chip8.cpp
 * Copyright (C) 2018 transpalette <transpalette@arch-cactus>
 *
 * Distributed under terms of the MIT license.
 */

#include "Chip8.h"
#include "Timer.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>

Chip8::Chip8()
{
    mRunning = false;
    mCpu = new CPU();
    mRam = Memory::GetInstance();
}

Chip8::~Chip8()
{
    delete mCpu;
    delete mRam; // Is this safe ?
}

void
Chip8::Init()
{
    /* Init registers and memory */
    /* Init fonts ? */
}

void
Chip8::Cycle()
{
    /* Run next instruction */
    printf("[*] Running one cycle\n");
    mCpu->Step();
    std::cin.get();
}

void
Chip8::UpdateTimers()
{
    Timer chip8Timers;
    chip8Timers.Start();
    while (this->mRunning) {
	if (chip8Timers.ElpasedMilliseconds() >= 16) {
	    mCpu->UpdateTimers();
	}
    }
    chip8Timers.Stop();
}

void
Chip8::DrawGraphics()
{
    // Write the pixels array using ncurses
}

void
Chip8::SetKeys()
{
    /* meh */
}

void
Chip8::MainLoop()
{
    Timer displayTimer;
    displayTimer.Start();
    while (this->mRunning) {
	Cycle();
	/* Try to refresh the screen at a 60Hz rate */
	if (displayTimer.ElpasedMilliseconds() >= 16) {
	    DrawGraphics();
	    displayTimer.Reset();
	}
	SetKeys();
    }
    displayTimer.Stop();
}

void
Chip8::Run()
{
    printf("[*] Running...\n");
    if (!mRunning) {
	mRunning = true;
	/* Start the mainLoop thread */
	std::thread tMainLoop(&Chip8::MainLoop, this);
	tMainLoop.join();
	/* Start the timer thread */
	std::thread tTimers(&Chip8::UpdateTimers, this);
	tTimers.join();
    }
}

void
Chip8::Stop()
{
    mRunning = false;
}

/* Load a CHIP-8 program - from a file - into the RAM */
void
Chip8::Load(const char* file_name)
{
    FILE* file = fopen(file_name, "rb");
    if (file != NULL) {
	uint8_t* bytes;
	/* Read file size */
	fseek(file, 0, SEEK_END);
	size_t file_size = ftell(file);
	rewind(file);

	/* Read the whole file as a block */
	bytes = (uint8_t*)malloc(sizeof(uint8_t) * file_size);
	size_t size_read = fread(bytes, 1, file_size, file);

	if (size_read != file_size) {
	    printf("[!] ERROR: could not read %s\n", file_name);
	    exit(1);
	}

	if (!mRam->WriteBytes(0x200, size_read, bytes)) {
	    printf("[!] ERROR: could not write the game in the RAM\n");
	    exit(1);
	}

	fclose(file);
	free(bytes);
    }
}
