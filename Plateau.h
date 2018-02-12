#ifndef PLATEAU_H
#define PLATEAU_H

#include <SFML/Graphics.hpp>

#include "Case.h"

#define NB_CASE 10

class Plateau : public sf::Drawable, public sf::Transformable
{
    public:
        Plateau();
        virtual ~Plateau();

    protected:

    private:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            // on applique la transformation de l'entité -- on la combine avec celle qui a été passée par l'appelant
            states.transform *= getTransform(); // getTransform() est définie par sf::Transformable

            // on applique la texture
            //states.texture = &m_texture;

            // on peut aussi surcharger states.shader ou states.blendMode si nécessaire

            // on dessine le tableau de vertex
            target.draw(_vertices, states);
        }

    private:
        sf::VertexArray _vertices;

        Case _plateau[NB_CASE][NB_CASE];
};

#endif // PLATEAU_H
