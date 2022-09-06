#include <SFML/Window.hpp>

#include <vector>
#include <string>

#include "world.hpp"

int main(int argc, char **argv) {
    sf::RenderWindow window(sf::VideoMode(WINDOWWIDTH, WINDOWHEIGHT),
                            "Karel World Sample");

    createDefaultWorld();
    std::vector<std::string>instructions;
    Karel karel(instructions);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::Black);
        renderWorld(window, karel);
        window.display();
    }

}
