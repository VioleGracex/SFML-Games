#include "game.hpp"
#include "menu.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath> // Include cmath for fmod function

Game::Game(sf::RenderWindow& window, Menu& menu)
    : window(window), menu(menu), birdVelocity(0), gameRunning(false), gameOver(false), currentFrame(0),
    lastPipeXPosition(window.getSize().x + 300), gap(200.0f), score(0) {
    window.setFramerateLimit(60);

    // Load bird textures
    if (!birdTextures[0].loadFromFile(RESOURCES_PATH "assets/GameObjects/yellowbird-downflap.png") ||
        !birdTextures[1].loadFromFile(RESOURCES_PATH "assets/GameObjects/yellowbird-midflap.png") ||
        !birdTextures[2].loadFromFile(RESOURCES_PATH "assets/GameObjects/yellowbird-upflap.png")) {
        // Handle error
    }
    birdSprite.setTexture(birdTextures[currentFrame]);
    birdSprite.setPosition(100, 300);

    // Load pipe texture
    if (!pipeTexture.loadFromFile(RESOURCES_PATH "assets/pipe.png")) {
        // Handle error
    }

    // Load background texture
    if (!backgroundTexture.loadFromFile(RESOURCES_PATH "assets/background.png")) {
        // Handle error
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        window.getSize().x / backgroundSprite.getLocalBounds().width,
        window.getSize().y / backgroundSprite.getLocalBounds().height
    );

    // Load font
    if (!font.loadFromFile(RESOURCES_PATH "assets/OpenSans-Regular.ttf")) {
        // Handle error
    }

    // Set up score text
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(window.getSize().x - 100, 10); // Position it in the top right corner

    // Set up game over text
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("Game Over\nClick to Return to Menu");
    gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2, window.getSize().y / 2 - gameOverText.getGlobalBounds().height / 2);
    gameOverText.setStyle(sf::Text::Bold);

    createPipes();
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        if (gameRunning) {
            update();
        }
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
            if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::W ||
                event.key.code == sf::Keyboard::Up || event.mouseButton.button == sf::Mouse::Left ||
                event.mouseButton.button == sf::Mouse::Right) {
                if (!gameRunning && !gameOver) {
                    gameRunning = true;
                    clock.restart();
                }
                birdVelocity = jumpVelocity;
            }

            if (gameOver) {
                reset();
                gameRunning = false;
                gameOver = false;
                // Return to menu
                menu.setActive(true);
            }
        }
    }
}

void Game::update() {
    float deltaTime = clock.restart().asSeconds();

    // Update bird animation
    if (animationClock.getElapsedTime().asSeconds() > 0.1f) {
        currentFrame = (currentFrame + 1) % 3;
        birdSprite.setTexture(birdTextures[currentFrame]);
        animationClock.restart();
    }

    // Update bird position
    birdVelocity += gravity * deltaTime;
    birdSprite.move(0, birdVelocity * deltaTime);

    // Update pipes
    for (auto& pipe : pipes) {
        pipe.move(-200 * deltaTime, 0);
    }

    if (pipes.size() > 1 && pipes[0].getPosition().x < -100) {
        pipes.erase(pipes.begin(), pipes.begin() + 2); // Remove both top and bottom pipes
        createPipes();
        increaseDifficulty(); // Increase difficulty as the player progresses
    }

    // Check for collision
    if (checkCollision()) {
        gameRunning = false;
        gameOver = true;
    }

    // Check if bird hits the ground or goes out of the screen
    if (birdSprite.getPosition().y > window.getSize().y || birdSprite.getPosition().y < 0) {
        gameRunning = false;
        gameOver = true;
    }

    // Update score text
    scoreText.setString("Score: " + std::to_string(score));
}

void Game::render() {
    window.clear();
    window.draw(backgroundSprite);
    for (const auto& pipe : pipes) {
        window.draw(pipe);
    }
    window.draw(birdSprite);
    window.draw(scoreText); // Draw score text

    if (gameOver) {
        window.draw(gameOverText); // Draw game over text if the game is over
    }

    window.display();
}

void Game::reset() {
    birdSprite.setPosition(100, 300);
    birdVelocity = 0;
    pipes.clear();
    createPipes();
    gameRunning = false;
    gameOver = false;
    currentFrame = 0;
    birdSprite.setTexture(birdTextures[currentFrame]);
    lastPipeXPosition = window.getSize().x + 300; // Reset the position for the first pipe
    gap = 200.0f; // Reset the gap
    score = 0; // Reset the score
}

void Game::createPipes() {
    std::srand(std::time(0)); // Seed randomness

    float pipeHeight = pipeTexture.getSize().y;
    float windowHeight = window.getSize().y;
    float playerHeight = birdTextures[0].getSize().y; // Get player sprite height
    float extraGap = 2 * playerHeight; // Extra space for fair difficulty

    int numPairs = 3; // Number of pipe pairs to spawn
    float minPipeDistance = 250.0f; // Minimum horizontal distance between pipe pairs
    float playerGapSize = playerHeight + extraGap; // Calculate gap for player dynamically

    for (int i = 0; i < numPairs; ++i) {
        // Randomize the gap
        float pipeGap = playerGapSize + (std::rand() % static_cast<int>(2 * playerGapSize - playerGapSize));

        // Create top pipe
        sf::Sprite pipeTop(pipeTexture);
        pipeTop.setRotation(180);
        pipeTop.setPosition(lastPipeXPosition, pipeHeight - pipeGap);

        // Create bottom pipe
        sf::Sprite pipeBottom(pipeTexture);
        pipeBottom.setPosition(lastPipeXPosition, (windowHeight - pipeHeight) + pipeGap);

        pipes.push_back(pipeTop);
        pipes.push_back(pipeBottom);

        // Move the next pipe pair forward
        lastPipeXPosition += minPipeDistance + (std::rand() % 100);
    }
}

bool Game::checkCollision() {
    for (const auto& pipe : pipes) {
        if (birdSprite.getGlobalBounds().intersects(pipe.getGlobalBounds())) {
            return true;
        }
    }
    return false;
}

bool Game::isGameOver() const {
    return gameOver;
}

void Game::increaseDifficulty() {
    score++;
    if (gap > 120.0f) { // Decrease the gap to a minimum of 120.0f
        gap -= 5.0f; // Decrease the gap as the player progresses
    }
}