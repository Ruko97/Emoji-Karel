#include <cassert>
#include "world.hpp"

bool Karel::move() {
    assert(i >= 0 && i < WORLDSIZE && j >= 0 && j < WORLDSIZE);

    const char *error_msg = "Error: can't move in the direction specified";

    switch (direction) {
    case top:
        if (i == 0 || World[i-1][j].bottom) {
            this->error_msg = error_msg;
            return false;
        }
        else {
            i--;
            return true;
        }
    case bottom:
        if (i == WORLDSIZE - 1 || World[i+1][j].top) {
            this->error_msg = error_msg;
            return false;
        }
        else {
            i++;
            return true;
        }
    case left:
        if (j == 0 || World[i][j-1].right) {
            this->error_msg = error_msg;
            return false;
        }
        else {
            j--;
            return true;
        }
    case right:
        if (j == WORLDSIZE - 1 || World[i][j+1].left) {
            this->error_msg = error_msg;
            return false;
        }
        else {
            j++;
            return true;
        }
    }
}
