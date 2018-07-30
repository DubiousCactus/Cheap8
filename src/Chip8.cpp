/*
 * Chip8.cpp
 * Copyright (C) 2018 transpalette <transpalette@arch-cactus>
 *
 * Distributed under terms of the MIT license.
 */

#include "Chip8.h"
#include "Keyboard.h"
#include "Timer.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>

Chip8::Chip8()
{
    mRunning = false;
    mScreen = Screen::GetInstance();
    mRam = Memory::GetInstance();
    mCpu = new CPU();
    Init();
}

Chip8::~Chip8()
{
    delete mCpu;
    delete mRam;
    delete mScreen;
}

void
Chip8::Init()
{
    /* Init fonts ? */
}

void
Chip8::UpdateTimers()
{
    Timer chip8Timers;
    chip8Timers.Start();
    while (this->mRunning) {
	if (chip8Timers.ElapsedMilliseconds() >= 16) {
	    mCpu->UpdateTimers();
	}
    }
    chip8Timers.Stop();
}


void
Chip8::MainLoop()
{
    Timer displayTimer, cpuTimer;
    displayTimer.Start();
    cpuTimer.Start();
    Keyboard::GetInstance()->StartListening();
    while (this->mRunning) {
	/* Run the CPU at 4MHz */
	if (cpuTimer.ElapsedNanoseconds() >= 250) {
	    /* Run next instruction */
	    mCpu->Step();
	    cpuTimer.Reset();
	}
	/* Try to refresh the screen at a 60Hz rate */
	if (displayTimer.ElapsedMilliseconds() >= 16) {
	    mScreen->Draw();
	    displayTimer.Reset();
	}
    }
    displayTimer.Stop();
    cpuTimer.Stop();
    Keyboard::GetInstance()->StopListening();
}

void
Chip8::Run()
{
    if (!mRunning) {
	mRunning = true;
	/* Start the mainLoop thread */
	std::thread(&Chip8::MainLoop, this).join();
	/* Start the timer thread */
	std::thread(&Chip8::UpdateTimers, this).join();
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
    if (file == nullptr) {
	std::cout << "[!] ERROR: Could not open '" << file_name << "'" << std::endl;
	exit(1);
    }

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
