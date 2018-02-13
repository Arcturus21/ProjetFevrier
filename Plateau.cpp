#include "Plateau.h"

Plateau::Plateau() : _vertices(sf::Quads, 4*NB_CASE*NB_CASE)
{
    //ctor
    sf::Color couleur=sf::Color::White;
    for(int i=0;i<NB_CASE;i++)
    {
        for(int j=0;j<NB_CASE;j++)
        {
            int indice= (i*10+j)*4;
            _vertices[indice].position = sf::Vector2f(i*TAILLE_CASE,j*TAILLE_CASE);
            _vertices[indice].color = couleur;
            _vertices[indice+1].position = sf::Vector2f((i+1)*TAILLE_CASE,j*TAILLE_CASE);
            _vertices[indice+1].color = couleur;
            _vertices[indice+2].position = sf::Vector2f((i+1)*TAILLE_CASE,(j+1)*TAILLE_CASE);
            _vertices[indice+2].color = couleur;
            _vertices[indice+3].position = sf::Vector2f(i*TAILLE_CASE,(j+1)*TAILLE_CASE);
            _vertices[indice+3].color = couleur;

            _plateau[i][j].init(i,j,couleur);

            if(couleur==sf::Color::Black)
                couleur=sf::Color::White;
            else
                couleur=sf::Color::Black;
        }
        if(couleur==sf::Color::Black)
            couleur=sf::Color::White;
        else
            couleur=sf::Color::Black;
    }
}

Plateau::~Plateau()
{
    //dtor
}
