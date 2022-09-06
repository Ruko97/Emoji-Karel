#include <stack>
#include <cstring>
#include <cassert>

#include <SFML/Window.hpp>

#include "world.hpp"

extern Box World[WORLDSIZE][WORLDSIZE];


//////////////////////////////////////////////////////////////////////////
// Instructions for Karel
//////////////////////////////////////////////////////////////////////////

#define OPCODEEQUALS(X) (strcmp(opcode, #X) == 0)

bool Karel::move() {
    assert(i >= 0 && i < WORLDSIZE && j >= 0 && j < WORLDSIZE);

    static char *const error_msg =
            "Error: can't move in the direction specified";

    pc++;           // Move program counter to next instruction
                    // Doing early cause returning is involved here

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
    pc++;           // Move program counter to next instruction
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
    pc++;           // Move program counter to next instruction
}

void Karel::notInstruction() {
    assert(accumulator == 0 || accumulator == 1);
    accumulator = 1 - accumulator;
    pc++;           // Move program counter to next instruction
}

void Karel::push() {
    accumulator_stack.push(accumulator);
    pc++;           // Move program counter to next instruction
}

void Karel::pop() {
    assert (!accumulator_stack.empty());
    accumulator_stack.pop();
    pc++;           // Move program counter to next instruction
}

void Karel::andInstruction() {
    assert(accumulator == 0 || accumulator == 1);
    accumulator = accumulator & accumulator_stack.top();
    assert(accumulator == 0 || accumulator == 1);
    pc++;           // Move program counter to next instruction
}

void Karel::orInstruction() {
    assert(accumulator == 0 || accumulator == 1);
    accumulator = accumulator | accumulator_stack.top();
    assert(accumulator == 0 || accumulator == 1);
    pc++;           // Move program counter to next instruction
}

void Karel::jz(int offset) {
    assert(accumulator == 0 || accumulator == 1);
    if (accumulator == 0) pc += offset;
    else pc++;
    assert(pc >= 0 && pc < instructions.size());
}

void Karel::jmp(int offset) {
    pc += offset;
    assert(pc >= 0 && pc < instructions.size());
}

void Karel::pushcount() {
    counter_stack.push(counter);
    counter = 0;
    pc++;           // Move program counter to next instruction
}

void Karel::popcount() {
    assert(!counter_stack.empty());
    counter = counter_stack.top();
    counter_stack.pop();
    pc++;           // Move program counter to next instruction
}

void Karel::jce(int count, int offset) {
    if (counter == count) pc += offset;
    else pc++;      // Move program counter to next instruction
    assert(pc >= 0 && pc < instructions.size());
}

void Karel::inc() {
    counter++;
    pc++;           // Move program counter to next instruction
}

void Karel::start() {
    reset();
    pc++;           // Move program counter to next instruction
}

void Karel::end() {
    // TODO: might need to fill something here
    // No jumping here, since this is end of program
}


/// Return true if instruction executed successfully, false if error
/// Boolean isMovement is set to true if the instruction was move or turnLeft,
/// is set to false otherwise
KarelState Karel::executeNextInstruction() {
    std::string currentInstruction = instructions[pc];

    // currentInstructionCstr is just the string currentInstruction
    // in char* form
    char *currentInstructionCstr =
            new char[currentInstruction.length() + 1];

    strcpy(currentInstructionCstr, currentInstruction.c_str());

    char *opcode = strtok(currentInstructionCstr, " ");

    if (OPCODEEQUALS(MOVE)) {
        bool result = move();
        if (result) state = moved;
        else state = error;
    }
    else if (OPCODEEQUALS(TURNLEFT)) {
        turnLeft();
        state = moved;
    }
    else {
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
        else if (OPCODEEQUALS(INC)) inc();
        else if (OPCODEEQUALS(START)) {
            assert(pc == 0);        // Asserting pc is 0 here
            assert(state == idle);  // Just adding a check here
            start();
        }
        else if (OPCODEEQUALS(END)) {
            assert(pc == instructions.size()-1);  // Asserting pc is at end
            state = KarelState::end;
            return state;   // We know the program has ended here, so there's
                            // no need to increment pc here
        }
        else {
            // ERROR
            fprintf(stderr, "ERROR: NOT SUPPOSED TO REACH THIS LINE!!");
            state = error;
            return state;
        }

        state = thought;
    }

    // dump();

    return state;   // All Ok, so return true
}

void Karel::reset() {
    i = WORLDSIZE-1;
    j = 0;
    direction = right;
    state = idle;
    error_msg = NULL;

    pc = 0;
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


void renderCrosshair(sf::RenderWindow &window, int i, int j) {
    float startx = PADDING + BOXSIZE * j + (BOXSIZE * 0.5)
            - (CROSSHAIRLENGTH * 0.5);
    float starty = PADDING + BOXSIZE * i + (BOXSIZE * 0.5)
            - (LINETHICKNESS * 0.5);

    sf::RectangleShape horizontalLine(
            sf::Vector2f(CROSSHAIRLENGTH, LINETHICKNESS));
    horizontalLine.setPosition(startx, starty);
    window.draw(horizontalLine);

    startx = PADDING + BOXSIZE * j + (BOXSIZE * 0.5)
            + (LINETHICKNESS * 0.5);
    starty = PADDING + BOXSIZE * i + (BOXSIZE * 0.5)
            - (CROSSHAIRLENGTH * 0.5);

    sf::RectangleShape verticalLine(
            sf::Vector2f(CROSSHAIRLENGTH, LINETHICKNESS));
    verticalLine.setPosition(startx, starty);
    verticalLine.rotate(90.f);
    window.draw(verticalLine);
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

    renderCrosshair(window, i, j);
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

        karelSprite.setOrigin(KARELIMAGEWIDTH * 0.5f, KARELIMAGEHEIGHT * 0.5f);

        spriteSetup = false;    // Set spriteSetup to be false
    }

    // Move the karel sprite to the required location
    // The 0.1 comes from 0.1 = 0.2 / 2 and because Karel's
    // image size = 0.8 * BOXSIZE
    const float starty = PADDING + BOXSIZE * i + 0.1 * BOXSIZE
            + 0.5 * KARELHEIGHT;
    const float startx = PADDING + BOXSIZE * j + 0.1 * BOXSIZE
            + 0.5 * KARELWIDTH;

    karelSprite.setPosition(startx, starty);

    karelSprite.rotate((int) direction * -90.f);

    window.draw(karelSprite);
}

void renderWorld(sf::RenderWindow &window, Karel &karel) {
    for (int i = 0; i < WORLDSIZE; i++) {
        for (int j = 0; j < WORLDSIZE; j++) {
            renderBox(window, i, j);
        }
    }
    karel.render(window);
    renderText(window, karel);
}

void renderText(sf::RenderWindow &window, Karel &karel) {
    static sf::Text text;

    static const float startx = PADDING;
    static const float starty = PADDING + WORLDSIZE * BOXSIZE + PADDING;

    if (karel.getErrorMessage() != NULL) {
        text.setFillColor(sf::Color::Red);
        text.setString(karel.getErrorMessage());
        text.setPosition(startx, starty);
        window.draw(text);
    }

    if (karel.getState() == KarelState::end) {
        text.setFillColor(sf::Color::Green);
        text.setString("Karel exectured successfully");
        text.setPosition(startx, starty);
        window.draw(text);
    }
}

KarelState Karel::executeUntilMovement() {
    do {
        executeNextInstruction();
    } while (state == thought || state == idle);

    assert(state == moved || state == error || state == KarelState::end);
    return state;
}

void Karel::dump() {
    if (!error_msg) {
        std::cerr << "ERROR: " << error_msg << std::endl;
    }
    std::cout << "PC: " << pc << std::endl;
    std::cout << "Current Instruction: " << instructions[pc] << std::endl;
    std::cout << "Karel's current state: " << state << std::endl;
    std::cout << "Karel's current direction: " << state << std::endl;
    std::cout << "Karel's location: [" << i << " row, " << j << " column]"
              << std::endl;
    std::cout << "Accumulator: " << accumulator << std::endl;
    std::cout << "Counter: " << counter << std::endl;
    std::cout << std::endl;
}
