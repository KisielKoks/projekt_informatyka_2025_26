#pragma once
#include <SFML/Graphics.hpp>
#include <array>

class Stone : public sf::RectangleShape
{
private:
    int m_punktyZycia;
    bool m_jestZniszczony;
    static const std::array<sf::Color, 4> m_colorLUT;

public:

    Stone(sf::Vector2f startPos, sf::Vector2f rozmiar, int L)
        : m_punktyZycia(L), m_jestZniszczony(false)
    {
        this->setPosition(startPos);
        this->setSize(rozmiar);
        this->setOutlineThickness(2.f);
        this->setOutlineColor(sf::Color::Black);
        aktualizujKolor();
    }

    void trafienie()
    {
        if (m_jestZniszczony)
            return;

        m_punktyZycia -= 1;
        aktualizujKolor();

        if (m_punktyZycia <= 0)
            m_jestZniszczony = true;
    }

    void aktualizujKolor()
    {
        if (m_punktyZycia >= 0 && m_punktyZycia < (int)m_colorLUT.size())
        {
            this->setFillColor(m_colorLUT[m_punktyZycia]);
        }
    }

    bool isDestroyed() const
    {
        return m_jestZniszczony;
    }

    void draw(sf::RenderTarget& target) const
    {
        if (!m_jestZniszczony)
            target.draw(*this);
    }

    int getHP() const { return m_punktyZycia; }
};
