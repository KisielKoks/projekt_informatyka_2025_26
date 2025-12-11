#include "Game.h"
#include <iostream>
#include <algorithm>

Game::Game()
    : m_paletka(400.f, 550.f, 100.f, 20.f, 8.f)
    , m_pilka(400.f, 300.f, 4.f, 3.f, 8.f)
    , m_gameOverShown(false)
    , m_savePressed(false)
    , m_paddleHits(0)
    , m_fontOk(false)
{

    m_fontOk = m_font.loadFromFile("arial.ttf");
    if (!m_fontOk)
    {
        std::cout << "Blad: nie udalo sie wczytac arial.ttf (hits)\n";
    }

    m_obstacle.setPointCount(5);
    m_obstacle.setPoint(0, { 0.f,   0.f });
    m_obstacle.setPoint(1, { 40.f,  5.f });
    m_obstacle.setPoint(2, { 30.f, 35.f });
    m_obstacle.setPoint(3, { 10.f, 45.f });
    m_obstacle.setPoint(4, { -5.f, 20.f });

    m_obstacle.setFillColor(sf::Color(100, 200, 250));
    m_obstacle.setOutlineThickness(2.f);
    m_obstacle.setOutlineColor(sf::Color::Black);
    m_obstacle.setPosition(200.f, 150.f);

    m_obstacleVelocity = { 120.f, 0.f };

    const int   ILOSC_KOLUMN = 6;
    const int   ILOSC_WIERSZY = 7;
    const float ROZMIAR_BLOKU_Y = 25.f;
    const float PRZERWA = 2.f;

    const float ROZMIAR_BLOKU_X =
        (WIDTH - (ILOSC_KOLUMN - 1) * PRZERWA) / ILOSC_KOLUMN;

    const float OFFSET_Y = 50.f;

    for (int w = 0; w < ILOSC_WIERSZY; ++w)
    {
        for (int k = 0; k < ILOSC_KOLUMN; ++k)
        {
            float posX = k * (ROZMIAR_BLOKU_X + PRZERWA);
            float posY = OFFSET_Y + w * (ROZMIAR_BLOKU_Y + PRZERWA);
            int rowFromBottom = ILOSC_WIERSZY - 1 - w;
            int hp = (rowFromBottom % 3) + 1;


            m_bloki.emplace_back(
                sf::Vector2f(posX, posY),
                sf::Vector2f(ROZMIAR_BLOKU_X, ROZMIAR_BLOKU_Y),hp);
        }
    }
}

void Game::update(sf::Time dt)
{
    float dtSec = dt.asSeconds();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
    {
        if (!m_savePressed)
        {
            m_savePressed = true;

            GameState snapshot;
            snapshot.capture(m_paletka, m_pilka, m_bloki);

            if (snapshot.saveToFile("savegame.txt"))
                std::cout << "Gra zapisana!\n";
            else
                std::cout << "Blad zapisu gry!\n";
        }
    }
    else
    {
        m_savePressed = false;
    }

    m_obstacle.move(m_obstacleVelocity * dtSec);

    sf::Vector2f obsPos = m_obstacle.getPosition();
    if (obsPos.x < 0.f || obsPos.x > WIDTH - 50.f)
    {
        m_obstacleVelocity.x = -m_obstacleVelocity.x;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        m_paletka.moveLeft();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        m_paletka.moveRight();
    }
    m_paletka.clampToBounds(WIDTH);

    m_pilka.move();
    m_pilka.collideWalls(WIDTH, HEIGHT);

    if (m_pilka.collidePaddle(m_paletka))
    {
        std::cout << "HIT PADDLE\n";
        std::cout << "x=" << m_pilka.getX()
            << " y=" << m_pilka.getY()
            << " vx=" << m_pilka.getVx()
            << " vy=" << m_pilka.getVy()
            << std::endl;

        m_paddleHits++;
    }

    float r = m_pilka.getRadius();
    sf::FloatRect ballBounds(
        m_pilka.getX() - r,
        m_pilka.getY() - r,
        2.f * r,
        2.f * r
    );

    for (Stone& blok : m_bloki)
    {
        if (blok.isDestroyed())
            continue;

        if (ballBounds.intersects(blok.getGlobalBounds()))
        {
            blok.trafienie();
            m_pilka.bounceY();
            break;
        }
    }

    m_bloki.erase(
        std::remove_if(m_bloki.begin(), m_bloki.end(),
            [](const Stone& s) { return s.isDestroyed(); }),
        m_bloki.end()
    );

    if (m_obstacle.getGlobalBounds().intersects(ballBounds))
    {
        m_pilka.bounceY();
    }

    if (!m_gameOverShown && m_pilka.getY() - m_pilka.getRadius() > HEIGHT)
    {
        std::cout << "MISS! KONIEC GRY\n";
        m_gameOverShown = true;
    }
}

void Game::render(sf::RenderTarget& target)
{
    for (const Stone& blok : m_bloki)
    {
        blok.draw(target);
    }

    target.draw(m_obstacle);

    m_paletka.draw(target);
    m_pilka.draw(target);

    if (m_fontOk)
    {
        sf::Text hits;
        hits.setFont(m_font);
        hits.setCharacterSize(24);
        hits.setFillColor(sf::Color::White);
        hits.setPosition(10.f, 10.f);
        hits.setString("Odbicia: " + std::to_string(m_paddleHits));

        target.draw(hits);
    }
}

bool Game::loadFromState(const GameState& state)
{
    const auto& pPos = state.getPaddlePosition();
    m_paletka.setPosition(pPos.x, pPos.y);

    const auto& bPos = state.getBallPosition();
    const auto& bVel = state.getBallVelocity();
    m_pilka.reset(bPos.x, bPos.y, bVel.x, bVel.y);

    m_bloki.clear();

    const int   ILOSC_KOLUMN = 6;
    const float ROZMIAR_BLOKU_Y = 25.f;
    const float PRZERWA = 2.f;
    const float ROZMIAR_BLOKU_X =
        (WIDTH - (ILOSC_KOLUMN - 1) * PRZERWA) / ILOSC_KOLUMN;

    for (const auto& bd : state.getBlocks())
    {
        m_bloki.emplace_back(
            sf::Vector2f(bd.x, bd.y),
            sf::Vector2f(ROZMIAR_BLOKU_X, ROZMIAR_BLOKU_Y),
            bd.hp
        );
    }

    m_gameOverShown = false;
    m_paddleHits = 0;

    return true;
}
