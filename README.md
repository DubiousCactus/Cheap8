# Cheap8

A CHIP-8 emulator written in C++ for learning and having fun!

[![asciicast](https://asciinema.org/a/194611.png)](https://asciinema.org/a/194611)

## Graphics

Cheap8 uses nothing but the ncurses library to provide old-school terminal graphics !

## Sound

This is still a work in progress...

## Input

No external library is used for the keyboard input (except ncurses), but a Linux-specific driver is coming soon to improve the responsiveness of the keyboard events...
The 16 keys of the hexadecimal keypad (on the original CHIP-8 machines) are mapped as-is: 0123456789abcdef !

## Misc.

The UI, the CPU, the sound/delay timers and the keyboard all run in separate threads to offer an as-smooth-as-possible experience !
