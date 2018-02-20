#ifndef PION_H
#define PION_H

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#define RAYON_PION 15

class Plateau;
class Case;

class Pion : public sf::Drawable, public sf::Transformable
{
    public:
        Pion(int caseX, int caseY, int tailleCase, sf::Color color);
        virtual ~Pion();

        bool IsAlive(){return _alive;};
        void SetAlive(bool alive){_alive=alive;};
        void SetPlateau(Plateau& p){_plateau=&p;};

        sf::Color GetColor(){return _color;};

        std::vector<Case*>* GetCaseDeplacement(bool manger=false);    ///Manger d�termine si le d�placement doit obligatoirement manger un pion adverse

        void setCase(int caseX,int caseY);
        Case* getCase();

    protected:

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            // on applique la transformation de l'entit� -- on la combine avec celle qui a �t� pass�e par l'appelant
            states.transform *= getTransform(); // getTransform() est d�finie par sf::Transformable

            // on applique la texture
            //states.texture = &m_texture;

            // on peut aussi surcharger states.shader ou states.blendMode si n�cessaire

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
