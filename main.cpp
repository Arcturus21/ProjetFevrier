#include <iostream>
#include <SFML/Graphics.hpp>

#include "Plateau.h"

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(TAILLE_CASE*NB_CASE, TAILLE_CASE*NB_CASE), "SFML works!");

    Plateau plateau;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(plateau);
        window.display();
    }

    return 0;
}
