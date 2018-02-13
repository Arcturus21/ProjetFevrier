#include "Case.h"
#include "Pion.h"

Case::Case(int indX, int indY, sf::Color color) :   _color(color),
    _indX(indX), _indY(indY), _position(indX*TAILLE_CASE,indY*TAILLE_CASE),
    _pionSurCase(NULL), _etatCase(vide)
{
    //ctor
}

Case::~Case()
{
    //dtor
}

void Case::init(int indX, int indY, sf::Color color)
{
    _indX = indX;
    _indY = indY;
    _position.x = _indX*TAILLE_CASE;
    _position.y = _indY*TAILLE_CASE;
    _pionSurCase = NULL;
    _etatCase = vide;
}

void Case::SetPion(Pion* p)
{
    if(p==NULL)
        return;
    _pionSurCase=p;
    if(p->GetColor()==sf::Color::Black)
        _etatCase=pionNoir;
    else
        _etatCase=pionBlanc;
}
