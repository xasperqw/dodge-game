#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Unit
{
    sf::CircleShape shape;
    sf::Vector2f target;
};

struct Bullet
{
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

class Game
{
public:
    Game();
    void run();

private:
    const float unitRadius = 30.f;
    const float bulletRadius = 10.f;
    const float unitSpeed = 220.f;
    const float bulletSpeed = 250.f;

    sf::RenderWindow window;
    sf::RectangleShape field;
    sf::CircleShape enemy;

    std::vector<Unit> units;
    std::vector<Bullet> bullets;

    int activeUnit = 0;
    int score = 0;
    bool gameOver = false;

    sf::Clock deltaClock;
    sf::Clock shootClock;

    sf::Font font;
    bool fontLoaded = false;

    sf::Text titleText;
    sf::Text scoreText;
    sf::Text gameOverText;

    void processEvents();
    void update(float dt);
    void render();
    void resetGame();
    void createUnits();
    void shoot();
    void moveUnits(float dt);
    void moveBullets(float dt);
    void updateText();
    void centerText(sf::Text& text, sf::Vector2f pos);

    float length(sf::Vector2f v);
    sf::Vector2f normalize(sf::Vector2f v);
    float distance(sf::Vector2f a, sf::Vector2f b);
};