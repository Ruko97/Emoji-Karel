#include <stack>
#include <cstring>
#include <cassert>

#include <SFML/Window.hpp>

#include "world.hpp"

#define LINETHICKNESS 5.f

extern Box World[WORLDSIZE][WORLDSIZE];


//////////////////////////////////////////////////////////////////////////
// Instructions for Karel
//////////////////////////////////////////////////////////////////////////

#define OPCODEEQUALS(X) (strcmp(opcode, #X) == 0)

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


/// Return true if instruction executed successfully, false if error
/// Boolean isMovement is set to true if the instruction was move or turnLeft,
/// is set to false otherwise
bool Karel::executeNextInstruction(bool &isMovement) {
    std::string currentInstruction = instructions[pc];

    // currentInstructionCstr is just the string currentInstruction
    // in char* form
    char *currentInstructionCstr =
            new char[currentInstruction.length() + 1];

    strcpy(currentInstructionCstr, currentInstruction.c_str());

    char *opcode = strtok(currentInstructionCstr, " ");

    if (OPCODEEQUALS(MOVE)) {
        bool result = move();
        isMovement = true;
        return result;
    }
    else if (OPCODEEQUALS(TURNLEFT)) {
        isMovement = true;
        turnLeft();
    }
    else {
        // In all the remaining cases, isMovement will be set to false
        isMovement = false;

        if (OPCODEEQUALS(FRONTBLOCKED)) frontBlocked();
        else if (OPCODEEQUALS(NOT)) notInstruction();
        else if (OPCODEEQUALS(PUSH)) push();
        else if (OPCODEEQUALS(POP)) pop();
        else if (OPCODEEQUALS(AND)) andInstruction();
        else if (OPCODEEQUALS(OR)) orInstruction();
        else if (OPCODEEQUALS(JZ)) {
            char *offsetStr = strtok(NULL, " ");
            assert(offsetStr != NULL && strcmp(offsetStr, "\n") != 0);
            int offset = atoi(offsetStr);
            jz(offset);
        }
        else if (OPCODEEQUALS(JMP)) {
            char *offsetStr = strtok(NULL, " ");
            assert(offsetStr != NULL && strcmp(offsetStr, "\n") != 0);
            int offset = atoi(offsetStr);
            jmp(offset);
        }
        else if (OPCODEEQUALS(PUSHCOUNT)) pushcount();
        else if (OPCODEEQUALS(POPCOUNT)) popcount();
        else if (OPCODEEQUALS(JCE)) {
            char *countStr = strtok(NULL, " ");
            assert(countStr != NULL && strcmp(countStr, "\n") != 0);
            int count = atoi(countStr);

            char *offsetStr = strtok(NULL, " ");
            assert(offsetStr != NULL && strcmp(offsetStr, "\n") != 0);
            int offset = atoi(offsetStr);

            jce(count, offset);
        }
        else {
            // ERROR
            fprintf(stderr, "ERROR: NOT SUPPOSED TO REACH THIS LINE!!");
            return false;
        }
    }

    return true;    // All Ok, so return true
}

void Karel::reset() {
    i = WORLDSIZE-1;
    j = 0;
    direction = right;

    pc = 0;    // or maybe 1, not sure
    accumulator = 0, counter = 0;
    while (!accumulator_stack.empty()) accumulator_stack.pop();
    while (!counter_stack.empty()) counter_stack.pop();
}


///////////////////////////////////////////////////////////////////////
// Rendering Functions
///////////////////////////////////////////////////////////////////////

void createDefaultWorld() {
    // TODO: add memset here

    for (int i = 0; i < WORLDSIZE; i++) {
        World[i][0].left = 1;
        World[i][WORLDSIZE-1].right = 1;
    }
    for (int j = 0; j < WORLDSIZE; j++) {
        World[0][j].top = 1;
        World[WORLDSIZE-1][j].bottom = 1;
    }
}

void renderBox(sf::RenderWindow &window, int i, int j) {
    const float starty = PADDING + BOXSIZE * i;
    const float startx = PADDING + BOXSIZE * j;

    Box &currentBox = World[i][j];

    if (currentBox.top) {
        sf::RectangleShape topLine(sf::Vector2f(BOXSIZE, LINETHICKNESS));
        topLine.setPosition(startx, starty);
        window.draw(topLine);
    }
    if (currentBox.bottom) {
        sf::RectangleShape bottomLine(sf::Vector2f(BOXSIZE, LINETHICKNESS));
        bottomLine.setPosition(startx, starty + BOXSIZE);
        window.draw(bottomLine);
    }
    if (currentBox.left) {
        sf::RectangleShape leftLine(sf::Vector2f(BOXSIZE, LINETHICKNESS));
        leftLine.setPosition(startx, starty);
        leftLine.rotate(90.f);
        window.draw(leftLine);
    }
    if (currentBox.right) {
        sf::RectangleShape rightLine(sf::Vector2f(BOXSIZE, LINETHICKNESS));
        rightLine.setPosition(startx + BOXSIZE, starty);
        rightLine.rotate(90.f);
        window.draw(rightLine);
    }
}

void Karel::render(sf::RenderWindow &window) {
    // Here, karelTexture and karelSprites are singletons, that can be
    // repeatedly used in the program instead of creating the sprite
    // over and over again whenever renderKarel is called

    static bool spriteSetup = false;    // a flag to make sure karelTexture
                                        // and karelSprite are setup only once
    static bool anyError = false;

    static sf::Texture karelTexture;
    static sf::Sprite karelSprite;

    if (anyError) return;

    if (!spriteSetup) {
        if (!karelTexture.loadFromFile("karelLarge.png",
                    sf::IntRect(0, 0, KARELIMAGEWIDTH, KARELIMAGEHEIGHT))) {
            fprintf(stderr, "ERROR: Karel image not found!!");
            anyError = true;
            return;
        }

        karelSprite.setTexture(karelTexture);

        // Scale karelSprite from the size of its image to the size we need
        karelSprite.setScale(
            ((float) KARELWIDTH) / KARELIMAGEWIDTH,
            ((float) KARELHEIGHT) / KARELIMAGEHEIGHT
        );

        spriteSetup = false;    // Set spriteSetup to be false
    }

    // Move the karel sprite to the required location
    // The 0.1 comes from 0.1 = 0.2 / 2 and because Karel's
    // image size = 0.8 * BOXSIZE
    const float starty = PADDING + BOXSIZE * i + 0.1 * BOXSIZE;
    const float startx = PADDING + BOXSIZE * j + 0.1 * BOXSIZE;

    karelSprite.setPosition(startx, starty);

    window.draw(karelSprite);
}

void renderWorld(sf::RenderWindow &window, Karel &karel) {
    for (int i = 0; i < WORLDSIZE; i++) {
        for (int j = 0; j < WORLDSIZE; j++) {
            renderBox(window, i, j);
        }
    }
    karel.render(window);
}

bool Karel::executeUntilMovement() {
    bool isMovement = false;
    bool result = false;

    do {
        result |= executeNextInstruction(isMovement);
    } while (!isMovement);

    return result;
}
