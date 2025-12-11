#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"
#include "Menu.h"
#include "GameState.h"

// zmiana nazwy enuma żeby nie gryzł się z klasą GameState
enum class AppState { Menu, Playing, Scores, Exiting };

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Arkanoid");
    window.setFramerateLimit(60);

    Menu menu(window.getSize().x, window.getSize().y);
    Game game;

    AppState currentState = AppState::Menu;
    sf::Clock deltaClock;

    while (window.isOpen())
    {
        sf::Time dt = deltaClock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (currentState == AppState::Menu)
                {
                    if (event.key.code == sf::Keyboard::Up)
                    {
                        menu.przesunG();
                    }
                    else if (event.key.code == sf::Keyboard::Down)
                    {
                        menu.przesunD();
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        int sel = menu.getSelectedItem();
                        if (sel == 0) // Nowa gra
                        {
                            game = Game();                // restart gry
                            currentState = AppState::Playing;
                        }
                        else if (sel == 1) // Wczytaj gre
                        {
                            GameState loaded;
                            if (loaded.loadFromFile("savegame.txt"))
                            {
                                game.loadFromState(loaded);
                                currentState = AppState::Playing;
                                std::cout << "Gra wczytana!\n";
                            }
                            else
                            {
                                std::cout << "Blad wczytywania savegame.txt\n";
                            }
                        }
                        else if (sel == 2) // Wyjscie
                        {
                            window.close();
                        }
                    }
                }
                else if (currentState == AppState::Playing)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        currentState = AppState::Menu;
                    }
                }
            }
        }

        if (currentState == AppState::Playing)
        {
            game.update(dt);
        }

        window.clear();

        if (currentState == AppState::Menu)
        {
            menu.draw(window);
        }
        else if (currentState == AppState::Playing)
        {
            game.render(window);
        }

        window.display();
    }

    return 0;
}