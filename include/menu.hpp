#pragma once

#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void handleInput(sf::Event event);
    bool isMenuActive() const;
    void setActive(bool active);  // Function to set menu active/inactive

private:
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
    sf::Texture messageTexture;
    sf::Sprite messageSprite;
    bool menuActive;
};