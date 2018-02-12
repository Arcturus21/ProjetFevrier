#ifndef PION_H
#define PION_H

#include <SFML/Graphics.hpp>

#define RAYON_PION 15

class Plateau;

class Pion : public sf::Drawable, public sf::Transformable
{
    public:
        Pion(int caseX, int caseY, int tailleCase, sf::Color color);
        virtual ~Pion();

        bool IsAlive(){return _alive;};
        void SetPlateau(Plateau& p){_plateau=&p;};

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
            target.draw(_shape, states);
        }

    private:
        int _caseX, _caseY;
        sf::Color _color;
        sf::CircleShape _shape;

        bool _alive;

        Plateau* _plateau;
};

#endif // PION_H
