#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Paletka.h"
#include "Pilka.h"
#include "Stone.h"
#include "GameState.h"

class Game
{
public:
    Game();

    // aktualizacja logiki gry
    void update(sf::Time dt);

    // rysowanie gry na podany target (okno)
    void render(sf::RenderTarget& target);

    // wczytanie stanu gry z GameState (jeœli u¿ywasz "Wczytaj gre" w menu)
    bool loadFromState(const GameState& state);

private:
    int      m_paddleHits;
    sf::Font m_font;
    sf::Text m_hitsText;
    bool     m_fontOk;
    static constexpr float WIDTH = 800.f;
    static constexpr float HEIGHT = 600.f;

    // obiekty gry
    Paletka         m_paletka;
    Pilka           m_pilka;
    std::vector<Stone> m_bloki;

    bool m_gameOverShown; // ¿eby "MISS! KONIEC GRY" by³o raz
    bool m_savePressed;   // ¿eby "Gra zapisana!" by³o raz na F5

    // ---- nieregularny kszta³t poruszaj¹cy siê poziomo ----
    sf::ConvexShape m_obstacle;
    sf::Vector2f    m_obstacleVelocity;
};
