#include "world.hpp"

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

    Karel.i = WORLDSIZE-1;
    Karel.j = 0;
    Karel.orientation = right;
}
