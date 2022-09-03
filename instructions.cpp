#include <cassert>
#include "world.hpp"

bool Karel::move() {
    assert(i >= 0 && i < WORLDSIZE && j >= 0 && j < WORLDSIZE);

    char *const error_msg = "Error: can't move in the direction specified";

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

void Karel::turnLeft() {
    direction = (enum Direction) ((direction + 1) % 4);
}

void Karel::frontBlocked() {
    switch (direction) {
    case top:
        if (i == 0 || World[i-1][j].bottom) accumulator = 1;
        else accumulator = 0;
        break;
    case bottom:
        if (i == WORLDSIZE - 1 || World[i+1][j].top) accumulator = 1;
        else accumulator = 0;
        break;
    case left:
        if (j == 0 || World[i][j-1].right) accumulator = 1;
        else accumulator = 0;
        break;
    case right:
        if (j == WORLDSIZE - 1 || World[i][j+1].left) accumulator = 1;
        else accumulator = 0;
        break;
    }
}

void Karel::notInstruction() {
    assert(accumulator == 0 || accumulator == 1);
    accumulator = 1 - accumulator;
}

void Karel::push() {
    accumulator_stack.push(accumulator);
}

void Karel::pop() {
    assert (!accumulator_stack.empty());
    accumulator_stack.pop();
}

void Karel::andInstruction() {
    assert(accumulator == 0 || accumulator == 1);
    accumulator = accumulator & accumulator_stack.top();
    assert(accumulator == 0 || accumulator == 1);
}

void Karel::orInstruction() {
    assert(accumulator == 0 || accumulator == 1);
    accumulator = accumulator | accumulator_stack.top();
    assert(accumulator == 0 || accumulator == 1);
}

void Karel::jz(int offset) {
    assert(accumulator == 0 || accumulator == 1);
    if (accumulator == 0) pc += offset;
    assert(pc >= 0 && pc < instructions.size());
}

void Karel::jmp(int offset) {
    pc += offset;
    assert(pc >= 0 && pc < instructions.size());
}

void Karel::pushcount() {
    counter_stack.push(counter);
    counter = 0;
}

void Karel::popcount() {
    assert(!counter_stack.empty());
    counter = accumulator_stack.top();
    accumulator_stack.pop();
}

void Karel::jce(int count, int offset) {
    if (counter == count) pc += offset;
    assert(pc >= 0 && pc < instructions.size());
}

void Karel::inc() {
    counter++;
}

void Karel::start() { reset(); }
void Karel::end() {}    // TODO: might need to fill something here
