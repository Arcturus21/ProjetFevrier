#include "Pion.h"

Pion::Pion(int caseX, int caseY, int tailleCase, sf::Color color):
    _caseX(caseX), _caseY(caseY), _color(color), _shape(RAYON_PION)
{
    //ctor
    setOrigin(RAYON_PION,RAYON_PION);
    _shape.setFillColor(color);
    _shape.setOutlineColor(sf::Color(128,128,128));
    _shape.setOutlineThickness(3);

    int moitTailleCase=tailleCase/2;
    setPosition(caseX*tailleCase+moitTailleCase,caseY*tailleCase+moitTailleCase);
}

Pion::~Pion()
{
    //dtor
}
