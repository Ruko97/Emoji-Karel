#pragma once

#include <iostream>
#include <vector>
#include <stack>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define WORLDSIZE 8

#define BOXSIZE 100
#define PADDING 10
#define FOOTERHEIGHT 100

#define LINETHICKNESS 5.f
#define CROSSHAIRLENGTH 15.f

// 2 * PADDING for padding on both sides (top and bottom; left and right)
#define WINDOWWIDTH (BOXSIZE * WORLDSIZE + 2 * PADDING)
// 3 * PADDING for paddding on both sides of the world and one after
// the footer text
#define WINDOWHEIGHT (BOXSIZE * WORLDSIZE + 3 * PADDING + FOOTERHEIGHT)

#define KARELIMAGEWIDTH 188
#define KARELIMAGEHEIGHT 188

#define KARELWIDTH (BOXSIZE * 4 / 5)
#define KARELHEIGHT (BOXSIZE * 4 / 5)

#define KARELIMAGENAME "assets/karelLarge.png"
#define FONTNAME "assets/arial.ttf"

#define TEXTSIZE 40


/*
        Structure of the world:

        j=0 -->
        _______________________________________
  i=0  |                                       |
   |   |                                       |
   V   |                                       |
       |                                       |
       |                                       |
       |                                       |
       |                                       |
       |                                       |
       |                                       |
       |                                       |
       |                                       |
       |  o                                    |
       | |_\-                                  |
       | | |                                   |
       |_______________________________________|

*/

struct Box {
    // If any of the variables are set to 1, it means that that edge of
    // the box is blocked and Karel can't cross it
    unsigned top : 1;
    unsigned bottom: 1;
    unsigned left : 1;
    unsigned right : 1;
};

// Here i indicates row and j indicates column;
//
//                  i          j
static Box World[WORLDSIZE][WORLDSIZE];

enum Direction { right = 0, top, left, bottom };
enum KarelState {
    error = -1, // There was some error in the last instruction
    idle,       // Karel hasn't started yet
    moved,      // Karel moved in the last instruction
    thought,    // Karel executee a instruction but didn't move
    end,        // Karel finished executing successfully
};

class Karel {
    int i, j;
    Direction direction;
    KarelState state;

    char *error_msg;

    std::vector<std::string> instructions;

    int pc;    // program counter
    unsigned int accumulator, counter;    // counter is the counter register
    std::stack<unsigned int> accumulator_stack, counter_stack;

    // List of all the instructions Karel can execute
    bool move();        // only the move instruction can result in an error
    void turnLeft();
    void frontBlocked();
    void notInstruction();
    void push();
    void pop();
    void andInstruction();
    void orInstruction();
    void jz(int offset);
    void jmp(int offset);
    void pushcount();
    void popcount();
    void jce(int count, int offset);
    void inc();
    void start();
    void end();
public:
    Karel(std::vector<std::string> &instructions)
            : instructions(std::move(instructions)) { reset(); }
    void reset();
    KarelState executeNextInstruction();
    const char* getErrorMessage() { return error_msg; }
    KarelState getState() { return state; }
    void render(sf::RenderWindow &window);

    /// Execute Karel::executeNextInstruction until a move or turnLeft has run
    KarelState executeUntilMovement();

    void dump();
};

void createDefaultWorld();

void renderBox(sf::RenderWindow &window, int i, int j);
void renderText(sf::RenderWindow &window, Karel &karel);
void renderWorld(sf::RenderWindow &window, Karel &karel);
void renderCrosshair(sf::RenderWindow &window, int i, int j);


