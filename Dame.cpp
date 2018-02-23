#include "Dame.h"
#include "Case.h"
#include "Plateau.h"

using namespace std;

Dame::Dame(const Pion& p) : Pion(p)
{
    //ctor
    _shape.setOutlineThickness(5);
    if(_color==sf::Color::White)
    {
        _shape.setOutlineColor(sf::Color(50,50,200));
    }
    else
    {
        _shape.setOutlineColor(sf::Color(200,50,50));
    }
}

Dame::~Dame()
{
    //dtor
}

std::vector<Case*>* Dame::GetCaseDeplacement(bool manger)
{
    vector<Case*>* retour=new vector<Case*>();
    Case *cTemp=NULL;

    EtatCase pionAdverse;
    int sensX=0, sensY=0;
    int indCActuelX=0, indCActuelY=0;
    if(_color==sf::Color::White)
    {
        pionAdverse=pionNoir;
    }
    else
    {
        pionAdverse=pionBlanc;
    }

    ///Vérifie si on peut manger un pion dans les 4 directions
    for(int i=0; i<4; i++)  ///Pour chaque diagonale
    {
        sensX = (i<2)? -1:1;        ///sens de parcours de la diagonale
        sensY = (i%2==0)? -1:1;
        indCActuelX=_caseX+sensX;
        indCActuelY=_caseY+sensY; ///indice actuel de la case

        cTemp =_plateau->GetCase(indCActuelX,indCActuelY);       ///Case sur laquelle on est actuellement
        while(cTemp!=NULL && cTemp->GetEtatCase()==vide) ///Tant qu'on a pas rencontré de pion
        {
            indCActuelX+=sensX;
            indCActuelY+=sensY;
            cTemp =_plateau->GetCase(indCActuelX,indCActuelY);   ///Case suivante
        }
        if(cTemp==NULL || cTemp->GetEtatCase()!=pionAdverse) ///Si on est en dehors du plateau, ou si on tombe sur un pion allié
            continue;                                        ///on arrête cette diagonale

        indCActuelX+=sensX;
        indCActuelY+=sensY;
        cTemp =_plateau->GetCase(indCActuelX,indCActuelY);
        while(cTemp!=NULL && cTemp->GetEtatCase()==vide)  ///On ajoute toutes les cases derrières
        {
            retour->push_back(cTemp);
            manger=true;
            indCActuelX+=sensX;
            indCActuelY+=sensY;
            cTemp =_plateau->GetCase(indCActuelX,indCActuelY);
        }
    }

    ///Si on a pas l'occasion de manger un pion, on peut déplacer librement
    if(!manger)
    {
        for(int i=0; i<4; i++)  ///Pour chaque diagonale
        {
            sensX = (i<2)? -1:1;        ///sens de parcours de la diagonale
            sensY = (i%2==0)? -1:1;
            indCActuelX=_caseX+sensX;
            indCActuelY=_caseY+sensY; ///indice actuel de la case

            cTemp =_plateau->GetCase(indCActuelX,indCActuelY);
            while(cTemp!=NULL && cTemp->GetEtatCase()==vide)  ///On ajoute toutes les cases derrières
            {
                retour->push_back(cTemp);
                indCActuelX+=sensX;
                indCActuelY+=sensY;
                cTemp =_plateau->GetCase(indCActuelX,indCActuelY);
            }
        }
    }

    return retour;
}
