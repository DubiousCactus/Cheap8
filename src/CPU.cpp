/*
 * CPU.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include "CPU.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

CPU::CPU()
{
  srand((unsigned)time(NULL));
  memset(mV, 0, 16);
  memset(mScreen, 0, SCREEN_H * SCREEN_W);
  mStack = new Stack();
  mRam = Memory::GetInstance();
  mI = 0;
  mPC = GAME_OFFSET;
  mOpcode = 0;
  mJMP = false;
}

CPU::~CPU()
{
  delete mStack;
  delete mRam;
}

void
CPU::Step()
{
  printf("[*] CPU step (PC=%02X): fetching opcode...\n", mPC);
  mOpcode = mRam->ReadOpCode(mPC); // Fetch next opcode in the RAM
  Execute();

  if (!mJMP)
    mPC += 2;
  else
    mJMP = false;
  // TODO: Use a timer to slow the CPU down till it's playable (what's the
  // reference ?)
}

void
CPU::ClearScreen()
{
  memset(mScreen, 0, SCREEN_H * SCREEN_W);
}

void
CPU::Draw(const uint8_t x, const uint8_t y, const uint8_t height)
{
  if ((x * y) < 0 || (x * y) > (SCREEN_H * SCREEN_W)) {
    std::cout << "[ERROR]: Out of screen coordinates!" << std::endl;
    exit(1);
  }

  mV[0xF] = 0;

  for (int i = 0; i < height; i++) {
    auto byte = mRam->ReadByte(mI + i);
    for (int j = 0; j < 8; j++) { // Draw a row
      auto mask = 1 << (8 - i);
      if (byte & mask) { // Flip the pixel
        if (mScreen[i * j] == 1)
          mV[0xF] = 1; // Collision!
        mScreen[i * j] ^= mScreen[i * j];
      }
    }
  }
}

uint8_t
CPU::GetDelay()
{
  return mDelay_timer;
}

uint8_t
CPU::GetKey()
{
  // TODO: returns the pressed key
  return 0;
}

void
CPU::SetDelay(uint8_t value)
{
  mDelay_timer = value;
}

void
CPU::SetSoundTimer(uint8_t value)
{
  mSound_timer = value;
}

uint16_t
CPU::GetSpriteAddr(uint8_t r)
{
  // TODO
  return 0x000;
}

/* Store V0 to VX (r - included) in memory, starting at address I */
void
CPU::DumpRegisters(int r_offset, uint16_t addr)
{
  for (int i = 0; i <= r_offset; i++)
    mRam->WriteByte(addr + i, mV[i]);
}

/* Fills V0 to VX (r - included) with values from RAM starting at address I
 */
void
CPU::LoadRegisters(int r_offset, uint16_t addr)
{
  for (int i = 0; i <= r_offset; i++)
    mV[i] = mRam->ReadByte(addr + i);
}

/* Stores the BCD representation of the given register into I */
void
CPU::SetBCD(uint8_t r)
{
  mRam->WriteByte(mI, r / 100);
  mRam->WriteByte(mI + 1, r / 10 % 10);
  mRam->WriteByte(mI + 2, r % 10);
}

void
CPU::Execute()
{
  printf("[*] Executing opcode: 0x%02X\n", mOpcode);

  switch (mOpcode & 0xF000) {
    case 0x0000:
      switch (mOpcode & 0x000F) {
        case 0x00E0:
          ClearScreen();
          break;
        case 0x00EE:
          return; // Return from subroutine. What does that imply actually ??
          break;
        default:
          break;
      }
      break;
    case 0x1000: // Jump to address
      mPC = mOpcode & 0x0FFF;
      mJMP = true;
      break;
    case 0x2000:         // Call subroutine
      mStack->Push(mPC); // Save PC on the stack
      mPC = mOpcode & 0x0FFF;
      mJMP = true;
      break;
    case 0xA000: // Set I = NNN
      mI = mOpcode & 0x0FFF;
      break;
    case 0xB000: // Jump to NNN + V0
      mPC = (mOpcode & 0x0FFF) + mV[0];
      mJMP = true;
      break;
    case 0xC000: // Set VX = NN & random()
      mV[mOpcode & 0x0F00] =
        (rand() % 255) &
        (mOpcode & 0x00FF); // TODO: Make sure this is done right!
      break;
    case 0xD000: // Draw a sprite at (VX, VY), that has a width of 8 pixels and
                 // a height of N pixels
      Draw(mV[mOpcode & 0x0F00], mV[mOpcode & 0x00F0], mOpcode & 0x000F);
      break;

    case 0xE000: {
      uint8_t x = ((mOpcode & 0x0F00) >> 8);
      if (x < 0 || x >= 16) {
        std::cout << "ERROR: invalid register (R" << x << ")" << std::endl;
        exit(1);
      }

      switch (mOpcode & 0xF0FF) {
        case 0xE09E: // Skip next instruction if the key in VX is pressed
          // TODO
          break;
        case 0xE0A1: // Skip next instruction if the key in VX isn't pressed
          // TODO
          break;
      }
      break;
    }

    case 0xF000: {
      uint8_t x = ((mOpcode & 0x0F00) >> 8);
      if (x < 0 || x >= 16) {
        std::cout << "ERROR: invalid register";
        exit(1);
      }

      switch (mOpcode & 0x00FF) {
        case 0x0007: // Set VX = delay timer
          mV[x] = GetDelay();
          break;
        case 0x000A: // Set VX = key press (blocking)
          mV[x] = GetKey();
          break;
        case 0x0015: // Set delay timer = VX
          SetDelay(mV[x]);
          break;
        case 0x0018: // Set sound timer = VX
          SetSoundTimer(mV[x]);
          break;
        case 0x001E: // Set mI += Vx
          mI += mV[x];
          break;
        case 0x0029: // Set I = location of the sprite for the character in VX
          mI = GetSpriteAddr(mV[x]);
          break;
        case 0x0033: // Store BCD of VX
          SetBCD(mV[x]);
          break;
        case 0x0055: // Dump V0 to VX in memory (starting at I)
          DumpRegisters(mV[x], mI);
          break;
        case 0x0065: // Fill V0 to VX from memory (starting at I)
          LoadRegisters(mV[x], mI);
          break;
        default:
          break;
      }
      break;
    }
    default:
      /* TODO: Refactor the next few opcodes, for 0x3000 to 0x9000 */
      if ((mOpcode & 0xF000) >= 0x3000 && (mOpcode & 0xF000) <= 0x9000) {
        short x = ((mOpcode & 0x0F00) >> 8); // Register index
        uint8_t value = mOpcode & 0x00FF;
        if (x < 0 || x >= 16) {
          printf("ERROR (op=0x%02X): invalid register (R%i)", mOpcode, x);
          exit(1);
        }

        switch (mOpcode & 0xF000) {
          case 0x3000: // Skip next mOpcode if VX = NN
            if (mV[x] == value)
              mPC += 2;
            break;
          case 0x4000: // Skip next opcode if VX != NN
            if (mV[x] != value)
              mPC += 2;
            break;
          case 0x6000: // Set VX = NN
            mV[x] = value;
            break;
          case 0x7000: // Set VX += NN (no carry flag)
            mV[x] += value;
            break;

          case 0x5000:
          case 0x8000:
          case 0x9000: // Refactor for 0x5XY0, 0x8XYN, 0x9XYN
          {
            short x = ((mOpcode & 0x0F00) >> 8), y = ((mOpcode & 0x00F0) >> 4);
            if (x < 0 || x >= 16 || y < 0 || y >= 16) {
              std::cout << "ERROR: invalid register";
              exit(1);
            }

            switch (mOpcode & 0xF00F) {
              case 0x5000: // Skip next opcode if VX == VY
                if (mV[x] == mV[y])
                  mPC += 2;
                break;
              /* TODO: Maybe move 0x8 to its own function ? */
              case 0x8000: // Set VX = VY
                mV[x] = mV[y];
                break;
              case 0x8001: // Set VX = (VX | VY)
                mV[x] = (mV[x] | mV[y]);
                break;
              case 0x8002: // Set VX = (VX & VY)
                mV[x] = (mV[x] & mV[y]);
                break;
              case 0x8003: // Set VX = (VX ^ VY)
                mV[x] = (mV[x] ^ mV[y]);
                break;
              case 0x8004:                  // Set VX += VY
                if ((mV[x] + mV[y]) > 0xFF) // Set VF to 1 if there's a carry
                  mV[0xF] = 1;

                mV[x] += mV[y];
                break;
              case 0x8005:         // Set VX -= VY
                if (mV[y] > mV[x]) // Set VF to 0 if there's a borrow
                  mV[0xF] = 0;

                mV[x] -= mV[y];
                break;
              case 0x8006: // Shift Vx = Vy >> 1
                // TODO: V[0xF] = LSB of VY (but remember BIG ENDIAN !!)
                mV[x] = mV[y] >> 1;
                break;
              case 0x8007:         // Set VX = VY - VX
                if (mV[x] > mV[y]) // Set VF to 0 if there's a borrow
                  mV[0xF] = 0;

                mV[x] = mV[y] - mV[x];
                break;
              case 0x800E: // Shift and set Vx = Vy = Vy << 1
                // TODO: mV[0xF] = MSB of V[y] (remember BIG ENDIAND)
                mV[y] <<= 1;
                mV[x] = mV[y];
                break;
              case 0x9000: // Skips the next instruction if VX != VY
                if (mV[x] != mV[y])
                  mPC += 2;
                break;
              default:
                break;
            }
            break;
          }
          default:
            break;
        }
      } else {
        printf("[WARNING] Unhandled opcode %02X\n", mOpcode);
      }
      break;
  }
}
