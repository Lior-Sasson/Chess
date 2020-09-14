# Chess

## Introduction

This is a standard chess game in C code with two options:
1. command line interface 
2. graphical user interface

The game can be played with two players or one player against the computer, in which case, it has 5 difficulty levels.
You can undo 6 times max (To change this, change MAX_SIZE in the History header), and save up to 5 games in memory.

## Requirements

In order to run the code, you must have:
- SDL2 2.0.5 : https://wiki.libsdl.org/FrontPage
  You must add SDL to your IDE's path.
- MinGw for C compilation

The current version was run and tested in a Windows machine, Eclipse Juno.

## How to use:

Create a project in Eclipse Juno.

To add SDL to Eclipse path, follow these steps:
1. Right click on the project, choose Properties.
2. Choose 'C/C++ General', and then 'Paths and Symbols'.
3. In 'Includes': choose 'Add' and add the path to your SDL folder. (for example, C:/ProgramFiles/SDL2-2.0.5)
4. In 'Libraries', choose 'Add' and write 'mingw32'. Then, do the same to add 'SDL2main' and 'SDL2', in that order.
5. In 'Library Paths', choose 'Add' and add the lib folder of your SDL, with the correct version 32 or 64 bit. (for example, C:/ProgramFiles/SDL2-2.0.5/x86-64-w64-mingw32/lib)
6. Press Apply to build the model and then OK.

You're ready to go!
