#ifndef PION_H
#define PION_H

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#define RAYON_PION 15

class Plateau;
class Case;

class Pion : public sf::Drawable, public sf::Transformable
{
    public:
        Pion(int caseX, int caseY, int tailleCase, sf::Color color);
        Pion(const Pion& p);
        virtual ~Pion();

        bool IsAlive(){return _alive;};
        void SetAlive(bool alive){_alive=alive;};
        void SetPlateau(Plateau& p){_plateau=&p;};

        sf::Color GetColor(){return _color;};

        virtual std::vector<Case*>* GetCaseDeplacement(bool manger=false);    ///Manger détermine si le déplacement doit obligatoirement manger un pion adverse

        void setCase(int caseX,int caseY);
        Case* getCase();


        ///GESTION DES FLUX
        virtual void Afficher(std::ostream &flux) const;

        ///GESTION DES PACKETS
        void Transmettre(sf::Packet &packet) const;
        void Recevoir(sf::Packet &packet);

    protected:

    protected:
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

    protected:
        int _caseX, _caseY;
        sf::Color _color;
        sf::CircleShape _shape;

        bool _alive;

        Plateau* _plateau;

        int _indice;

    private:
        static int cptIndice;
};

std::ostream& operator<<(std::ostream &flux, Pion const& pion);

sf::Packet& operator <<(sf::Packet& packet, const Pion& pion);
sf::Packet& operator >>(sf::Packet& packet, Pion& pion);

///GESTION FLUX SFML SPECIFIC
sf::Packet& operator <<(sf::Packet& packet, const sf::Color color);
sf::Packet& operator >>(sf::Packet& packet, sf::Color color);
sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2f vector2);
sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f vector2);

#endif // PION_H
