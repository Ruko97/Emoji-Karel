#include <SFML/Window.hpp>

#include <fstream>

#include "util.hpp"
#include "parser.hpp"
#include "world.hpp"

int main(int argc, char **argv) {
    SetupBinops();
    getNextToken();

    auto program = ParseProgram();

    const char *filename = argc >= 2 ? argv[1] : "temp.krl";

    std::ofstream ofs(filename, std::ios::out);
    if (!ofs)
        std::cerr << "Couldn't create temporary file" << std::endl;

    program->codegen(ofs);

    std::string instruction;
    std::vector<std::string> instructions;

    std::ifstream ifs(filename, std::ios::in);
    if (!ifs)
        std::cerr << "File named " << filename << "not found after adding "
                  << "the genrated program to it" << std::endl;

    while (!ifs.eof()) {
        std::getline(ifs, instruction);
        if (!(ifs.eof() && instruction.length() == 0))
            instructions.push_back(instruction);
    }

    createDefaultWorld();
    Karel karel(instructions);

    sf::RenderWindow window(sf::VideoMode(WINDOWWIDTH, WINDOWHEIGHT),
                            "Emoji Karel");
    window.setKeyRepeatEnabled(false);
    bool finishedExecution = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    if (!finishedExecution) {
                        karel.executeUntilMovement();
                        if (karel.getState() == end) finishedExecution = true;
                        else if (karel.getState() == error) {
                            // TODO: enter error handling code here
                            finishedExecution = true;
                        }
                    } else {
                        // Finished execution, so close
                        // TODO: also show text showing that karel executed OK
                        window.close();
                    }
                }
            }
        }

        window.clear(sf::Color::Black);
        renderWorld(window, karel);
        window.display();
    }

    return 0;
}
