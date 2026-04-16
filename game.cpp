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

void Game::run()
{
    while (window.isOpen())
    {
        float dt = deltaClock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        else if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->scancode == sf::Keyboard::Scancode::Num1)
                activeUnit = 0;
            else if (key->scancode == sf::Keyboard::Scancode::Num2)
                activeUnit = 1;
            else if (key->scancode == sf::Keyboard::Scancode::Num3)
                activeUnit = 2;
            else if (key->scancode == sf::Keyboard::Scancode::R && gameOver)
                resetGame();
        }
        else if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (!gameOver && mouse->button == sf::Mouse::Button::Right)
            {
                sf::Vector2f click(
                    static_cast<float>(mouse->position.x),
                    static_cast<float>(mouse->position.y)
                );

                if (field.getGlobalBounds().contains(click))
                    units[activeUnit].target = click;
            }
        }
    }
}

void Game::update(float dt)
{
    if (!gameOver)
    {
        moveUnits(dt);

        if (shootClock.getElapsedTime().asSeconds() >= 1.f)
        {
            shoot();
            shootClock.restart();
        }

        moveBullets(dt);
    }

    updateText();
}

void Game::render()
{
    window.clear(sf::Color(20, 25, 35));
    window.draw(field);
    window.draw(enemy);

    for (std::size_t i = 0; i < units.size(); ++i)
    {
        units[i].shape.setOutlineThickness(i == static_cast<std::size_t>(activeUnit) ? 4.f : 0.f);
        units[i].shape.setOutlineColor(sf::Color::Yellow);
        window.draw(units[i].shape);

        if (fontLoaded)
        {
            sf::Text label(font, std::to_string(i + 1), 24);
            label.setFillColor(sf::Color::Black);
            centerText(label, units[i].shape.getPosition());
            window.draw(label);
        }
    }

    for (const auto& b : bullets)
        window.draw(b.shape);

    if (fontLoaded)
    {
        window.draw(titleText);
        window.draw(scoreText);

        if (gameOver)
            window.draw(gameOverText);
    }

    window.display();
}

void Game::resetGame()
{
    score = 0;
    activeUnit = 0;
    gameOver = false;
    bullets.clear();
    shootClock.restart();

    enemy.setPosition({ 400.f, 150.f });
    enemy.setFillColor(sf::Color(200, 70, 70));

    createUnits();
    updateText();
}

void Game::createUnits()
{
    units.clear();

    sf::Vector2f pos[3] = {
        {220.f, 420.f},
        {400.f, 450.f},
        {580.f, 420.f}
    };

    sf::Color colors[3] = {
        sf::Color(80, 180, 255),
        sf::Color(110, 230, 120),
        sf::Color(245, 200, 90)
    };
