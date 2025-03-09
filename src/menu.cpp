#include "menu.hpp"

Menu::Menu(sf::RenderWindow& window) : menuActive(true) {
    if (!backgroundTexture.loadFromFile(RESOURCES_PATH "assets/background.png")) {
        // Handle error
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        window.getSize().x / backgroundSprite.getLocalBounds().width,
        window.getSize().y / backgroundSprite.getLocalBounds().height
    );

    if (!messageTexture.loadFromFile(RESOURCES_PATH "assets/UI/message.png")) {
        // Handle error
    }
    messageSprite.setTexture(messageTexture);
    messageSprite.setPosition(window.getSize().x / 2 - messageSprite.getGlobalBounds().width / 2, window.getSize().y / 2 - messageSprite.getGlobalBounds().height / 2);
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    window.draw(messageSprite);
}

void Menu::handleInput(sf::Event event) {
    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
        menuActive = false; // Start game
    }
}

bool Menu::isMenuActive() const {
    return menuActive;
}

void Menu::setActive(bool active) {
    menuActive = active;
}