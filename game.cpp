#include "Game.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <optional>
#include <string>
#include <cstddef>

Game::Game()
    : window(
        sf::VideoMode({ 800, 600 }),
        "Microcontrol Game",
        sf::Style::Titlebar | sf::Style::Close
    ),
    titleText(font, "Microcontrol Game", 30),
    scoreText(font, "", 22),
    gameOverText(font, "Game Over", 42)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    window.setFramerateLimit(60);

    field.setSize({ 680.f, 420.f });
    field.setPosition({ 60.f, 90.f });
    field.setFillColor(sf::Color(35, 45, 65));
    field.setOutlineThickness(3.f);
    field.setOutlineColor(sf::Color(180, 190, 220));

    enemy.setRadius(unitRadius);
    enemy.setOrigin({ unitRadius, unitRadius });
    enemy.setPosition({ 400.f, 150.f });
    enemy.setFillColor(sf::Color(200, 70, 70));

    fontLoaded = font.openFromFile("assets/Curtsweeper-Regular.otf");

    resetGame();
}
