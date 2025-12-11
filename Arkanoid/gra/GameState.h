#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <string>

#include "Paletka.h"
#include "Pilka.h"
#include "Stone.h"

struct BlockData
{
    float x, y;
    int hp;
};

class GameState
{
private:
    sf::Vector2f paddlePosition;
    sf::Vector2f ballPosition;
    sf::Vector2f ballVelocity;
    std::vector<BlockData> blocksData;

public:
    // zrzut stanu z aktualnej gry
    void capture(const Paletka& pal, const Pilka& ball, const std::vector<Stone>& blocks)
    {
        paddlePosition = { pal.getX(), pal.getY() };
        ballPosition = { ball.getX(), ball.getY() };
        ballVelocity = { ball.getVx(), ball.getVy() };

        blocksData.clear();
        blocksData.reserve(blocks.size());

        for (const auto& b : blocks)
        {
            if (b.isDestroyed())
                continue;

            BlockData data;
            data.x = b.getPosition().x;
            data.y = b.getPosition().y;
            data.hp = b.getHP();

            blocksData.push_back(data);
        }
    }

    const sf::Vector2f& getPaddlePosition() const { return paddlePosition; }
    const sf::Vector2f& getBallPosition()   const { return ballPosition; }
    const sf::Vector2f& getBallVelocity()   const { return ballVelocity; }
    const std::vector<BlockData>& getBlocks() const { return blocksData; }

    // --- ZAPIS DO PLIKU ---
    bool saveToFile(const std::string& filename) const
    {
        std::ofstream file(filename);
        if (!file.is_open())
            return false;

        // linia 1: PADDLE x y
        file << "PADDLE " << paddlePosition.x << " " << paddlePosition.y << "\n";

        // linia 2: BALL x y vx vy
        file << "BALL "
            << ballPosition.x << " " << ballPosition.y << " "
            << ballVelocity.x << " " << ballVelocity.y << "\n";

        // linia 3: BLOCKS_COUNT n
        file << "BLOCKS_COUNT " << blocksData.size() << "\n";

        // kolejne linie: BLOCK x y hp
        for (const auto& b : blocksData)
        {
            file << "BLOCK " << b.x << " " << b.y << " " << b.hp << "\n";
        }

        return true;
    }

    // --- ODCZYT Z PLIKU ---
    bool loadFromFile(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
            return false;

        std::string label;

        // PADDLE x y
        if (!(file >> label >> paddlePosition.x >> paddlePosition.y))
            return false;

        // BALL x y vx vy
        if (!(file >> label
            >> ballPosition.x >> ballPosition.y
            >> ballVelocity.x >> ballVelocity.y))
            return false;

        // BLOCKS_COUNT n
        int blocksCount = 0;
        if (!(file >> label >> blocksCount))
            return false;

        blocksData.clear();
        blocksData.reserve(blocksCount);

        for (int i = 0; i < blocksCount; ++i)
        {
            BlockData data;
            if (!(file >> label >> data.x >> data.y >> data.hp))
                return false;

            blocksData.push_back(data);
        }

        return true;
    }
};