#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Menu;  // Forward declaration of Menu class

class Game {
public:
    Game(sf::RenderWindow& window, Menu& menu);
    void run();
    bool isGameOver() const;  // Function to check if the game is over

private:
    void processEvents();
    void update();
    void render();
    void reset();
    void createPipes();
    bool checkCollision();
    void increaseDifficulty();

    sf::RenderWindow& window;
    Menu& menu;  // Reference to Menu class

    sf::Texture birdTextures[3];
    sf::Sprite birdSprite;
    sf::Texture pipeTexture;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    std::vector<sf::Sprite> pipes;
    sf::Clock clock;
    sf::Clock animationClock;
    int currentFrame;
    float birdVelocity;
    const float gravity = 500.0f;
    const float jumpVelocity = -300.0f;
    bool gameRunning;
    bool gameOver;
    float lastPipeXPosition;
    float gap;
    int score;

    // Add font and text for score and game over message
    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
};