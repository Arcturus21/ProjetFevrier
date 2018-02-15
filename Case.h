#ifndef CASE_H
#define CASE_H

#include <SFML/Graphics.hpp>


#define TAILLE_CASE 40

class Plateau;
class Pion;


typedef enum
{
    vide,
    pionBlanc,
    pionNoir
} EtatCase;

class Case
{
public:
    Case(int indX=-1, int indY=-1, sf::Color color=sf::Color::Blue);
    virtual ~Case();

    void init(int indX, int indY, sf::Color color);

    void SetPion(Pion* p);
    Pion* GetPion(){return _pionSurCase;};
    EtatCase GetEtatCase()
    {
        return _etatCase;
    };
    sf::Vector2f GetPosition(){return _position;};
    int GetIndX(){return _indX;};
    int GetIndY(){return _indY;};

protected:

private:
    sf::Color _color;
    int _indX, _indY;
    sf::Vector2f _position;

    Pion* _pionSurCase;

    EtatCase _etatCase;
};

#endif // CASE_H
