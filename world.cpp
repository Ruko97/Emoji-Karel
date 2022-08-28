#include <stack>

#include "world.hpp"

extern Box World[WORLDSIZE][WORLDSIZE];

void Karel::reset() {
    i = WORLDSIZE-1;
    j = 0;
    direction = right;

    pc = 0;    // or maybe 1, not sure
    accumulator = 0, counter = 0;
    while (!accumulator_stack.empty()) accumulator_stack.pop();
    while (!counter_stack.empty()) counter_stack.pop();
}

void createDefaultWorld() {
    // TODO: add memset here

    for (int i = 0; i < WORLDSIZE; i++) {
        World[i][0].left = 1;
        World[i][WORLDSIZE-1].right = 1;
    }
    for (int j = 0; j < WORLDSIZE; j++) {
        World[0][j].top = 1;
        World[0][WORLDSIZE-1].bottom = 1;
    }

    karel.reset();
}
