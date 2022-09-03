#pragma once

#include <iostream>
#include <vector>
#include <stack>

#define WORLDSIZE 8

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
//      i          j
Box World[WORLDSIZE][WORLDSIZE];

enum Direction { top = 0, left, bottom, right };

class Karel {
    int i, j;
    Direction direction;

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
    Karel() { reset(); }
    Karel(std::vector<std::string> &instructions)
            : instructions(std::move(instructions)) { reset(); }
    void reset();
    bool executeNextInstruction();
    const char* getErrorMessage() { return error_msg; }
};

Karel karel;

void createDefaultWorld();
