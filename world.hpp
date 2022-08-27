#pragma once

#include <iostream>
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
    unsigned top : 1;
    unsigned bottom: 1;
    unsigned left : 1;
    unsigned right : 1;
};

// Here i indicates row and j indicates column;
//
//      i          j
Box[WORLDSIZE][WORLDSIZE] World;

enum Direction { top, bottom, left, right };

class Karel {
    int i, j;
    Direction direction;

    int pc;    // program counter
    unsigned int accumulator, counter;    // counter is the counter register
    std::stack<unsigned int> accumulator_stack,
        counter_stack;
public:
    Karel() { reset(); }
    void reset();
};

Karel karel;

void createDefaultWorld();
