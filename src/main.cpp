#include <SFML/Graphics.hpp>
#include "game.hpp"
#include "menu.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Flappy Bird");

    Menu menu(window);
    Game game(window, menu);  // Pass the menu to the game

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (menu.isMenuActive()) {
            menu.handleInput(event);
            window.clear();
            menu.draw(window);
            window.display();
        }
        else {
            game.run();
            if (game.isGameOver()) {
                menu.setActive(true);  // Return to menu after game over
            }
        }
    }

    return 0;
}