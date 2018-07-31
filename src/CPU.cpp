/*
 * CPU.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include "CPU.h"
#include "Screen.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

CPU::CPU(Keyboard* keyboard, Memory* ram, Screen* screen)
{
  srand((unsigned)time(NULL));
  memset(mV, 0, 16 * sizeof(mV[0]));
  mStack = new Stack();
  mRam = ram;
  mKeyboard = keyboard;
  mScreen = screen;
  mI = 0;
  mPC = GAME_OFFSET;
  mOpcode = 0;
  mJMP = false;
}

CPU::~CPU()
{
  delete mStack;
}

void
CPU::Step()
{
   //printw("[*] CPU step (PC=%02X): fetching opcode...\n", mPC);
  mOpcode = mRam->ReadOpCode(mPC); // Fetch next opcode in the RAM
  Execute();

  if (!mJMP)
    mPC += 2;
  else
    mJMP = false;
}

void
CPU::Draw(uint8_t x, uint8_t y, const uint8_t height)
{
  if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) {
    printf("[ERROR]: Out of screen coordinates! (%d, %d)", x, y);
    exit(1);
    if (x > SCREEN_WIDTH) {
      x -= SCREEN_WIDTH;
    } else if (x < 0) {
      x += SCREEN_WIDTH;
    }

    if (y > SCREEN_HEIGHT) {
      y -= SCREEN_HEIGHT;
    } else if (y < 0) {
      y += SCREEN_HEIGHT;
    }
  }

  mV[0xF] = 0;

  for (int i = 0; i < height; i++) {
    auto sprite = mRam->ReadByte(mI + i);
    for (int j = 0; j < 8; j++) { // Draw a row
      auto mask = 1 << (8 - (j + 1));
      if (sprite & mask) { // Flip the pixel
        if (mScreen->TogglePixel(x + j, y + i))
          mV[0xF] = 1; // Collision!
      }
    }
  }
}

uint8_t
CPU::GetDelay()
{
  return mDelay_timer;
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
CPU::GetSpriteAddr(uint8_t character)
{
  return FONTS_OFFSET + (5 * character);
}

/* Store V0 to VX (r - included) in memory, starting at address I */
void
CPU::DumpRegisters(int r_index, uint16_t addr)
{
  for (int i = 0; i <= r_index; i++)
    mRam->WriteByte(addr + i, mV[i]);
}

/* Fills V0 to VX (r - included) with values from RAM starting at address I
 */
void
CPU::LoadRegisters(int r_index, uint16_t addr)
{
  for (int i = 0; i <= r_index; i++)
    mV[i] = mRam->ReadByte(addr + i);
}

/* Stores the BCD representation of the given register into I */
void
CPU::SetBCD(uint8_t value)
{
  mRam->WriteByte(mI, value / 100);
  mRam->WriteByte(mI + 1, (value / 10) % 10);
  mRam->WriteByte(mI + 2, value % 10);
}

void
CPU::UpdateTimers()
{
  if (mDelay_timer > 0)
    mDelay_timer--;
  if (mSound_timer > 0)
    mSound_timer--;
}

void
CPU::Execute()
{
   //printw("[*] Executing opcode: 0x%02X\n", mOpcode);

  switch (mOpcode & 0xF000) {
    case 0x0000:
      switch (mOpcode & 0x00FF) {
        case 0x00E0: // Clear the screen
          mScreen->Clear();
          break;
        case 0x00EE: // Return from subroutine.
          mPC = mStack->Pop();
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
      mV[(mOpcode & 0x0F00) >> 8] = (rand() % 255) & (mOpcode & 0x00FF);
      break;
    case 0xD000: // Draw a sprite at (VX, VY), that has a width of 8 pixels and
                 // a height of N pixels
      Draw(mV[(mOpcode & 0x0F00) >> 8],
           mV[(mOpcode & 0x00F0) >> 4],
           mOpcode & 0x000F);
      break;

    case 0xE000: {
      uint8_t x = ((mOpcode & 0x0F00) >> 8);
      if (x < 0 || x >= 16) {
        std::cout << "ERROR: invalid register (R" << x << ")" << std::endl;
        exit(1);
      }

      switch (mOpcode & 0xF0FF) {
        case 0xE09E: // Skip next instruction if the key in VX is pressed
          printw("waiting for %d\n", mV[x]);
          if (mKeyboard->IsKeyPressed(mV[x]))
            mPC += 2;
          break;
        case 0xE0A1: // Skip next instruction if the key in VX isn't pressed
          printw("waiting for %d\n", mV[x]);
          if (!mKeyboard->IsKeyPressed(mV[x]))
            mPC += 2;
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
          printw("READING KEY\n");
          mV[x] = mKeyboard->ReadKey();
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
          DumpRegisters(x, mI);
          break;
        case 0x0065: // Fill V0 to VX from memory (starting at I)
          LoadRegisters(x, mI);
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
                mV[x] |= mV[y];
                break;
              case 0x8002: // Set VX = (VX & VY)
                mV[x] &= mV[y];
                break;
              case 0x8003: // Set VX = (VX ^ VY)
                mV[x] ^= mV[y];
                break;
              case 0x8004: // Set VX += VY
                mV[0xF] =
                  ((mV[x] + mV[y]) > 0xFF); // Set VF to 1 if there's a carry
                mV[x] += mV[y];
                break;
              case 0x8005:               // Set VX -= VY
                mV[0xF] = mV[x] > mV[y]; // Set VF to 0 if there's a borrow
                mV[x] -= mV[y];
                break;
              case 0x8006: // Shift Vx = Vx >> 1 (// TODO: Verify that it's not
                           // Vx = Vy >> 1)
                mV[0xF] = mV[x] & 0x01;
                mV[x] >>= 1;
                break;
              case 0x8007:               // Set VX = VY - VX
                mV[0xF] = mV[x] < mV[y]; // Set VF to 0 if there's a borrow
                mV[x] = mV[y] - mV[x];
                break;
              case 0x800E: // Shift and set Vx = Vx << 1
                mV[0xF] = ((mV[x] & 0x80) != 0);
                mV[x] <<= 1;
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
