#include <iostream>
#include <SFML/Graphics.hpp>

#include "Plateau.h"
#include "GameManager.h"

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(TAILLE_CASE*NB_CASE, TAILLE_CASE*NB_CASE), "SFML works!");

    Plateau plateau;

    GameManager gm;

    gm.Play(window);

    return 0;
}
