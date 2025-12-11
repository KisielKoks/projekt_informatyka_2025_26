#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Paletka.h"

class Pilka {
private:
    float x, y;
    float vx, vy;
    float radius;
    sf::CircleShape shape;

public:

    Pilka(float startX, float startY, float vx, float vy, float radius)
        : x(startX), y(startY), vx(vx), vy(vy), radius(radius) {

        shape.setRadius(radius);
        shape.setOrigin(radius, radius);
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::White);
    }

    void move() {
        x += vx;
        y += vy;
        shape.setPosition(x, y);
    }

    void bounceX() {
        vx = -vx;
    }

    void bounceY() {
        vy = -vy;
    }

    void collideWalls(float width, float height) {
        if (x - radius <= 0 || x + radius >= width) {
            bounceX();
            if (x - radius <= 0) x = radius;
            if (x + radius >= width) x = width - radius;
        }
        if (y - radius <= 0) {
            bounceY();
            y = radius;
        }
    }

    bool collidePaddle(const Paletka& p) {
        if (x >= p.getX() - p.getSzerokosc() / 2 && x <= p.getX() + p.getSzerokosc() / 2
            && (y + radius) >= (p.getY() - p.getWysokosc() / 2)
            && (y - radius) < (p.getY() - p.getWysokosc() / 2))
        {
            vy = -std::abs(vy);
            y = p.getY() - p.getWysokosc() / 2 - radius;
            return true;
        }
        return false;
    }

    void draw(sf::RenderTarget& target) {
        target.draw(shape);
    }

    void reset(float startX, float startY, float newVx, float newVy)
    {
        x = startX;
        y = startY;
        vx = newVx;
        vy = newVy;
        shape.setPosition(x, y);
    }

    void setVelocity(float nvx, float nvy) {
        vx = nvx;
        vy = nvy;
    }

    float getX() const { return x; }
    float getY() const { return y; }
    float getVx() const { return vx; }
    float getVy() const { return vy; }
    float getRadius() const { return radius; }
};